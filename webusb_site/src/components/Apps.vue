<template>
    <section id="apps">
        <mdb-row>
            <mdb-col md="6">
                <mdb-card class="mb-4">
                    <mdb-card-body class="badge_container">
                        <div class="header">
                            <div class=nav-left>
                                <mdb-btn color="primary" title="Previous Page" size="sm"><i class="fas fa-arrow-left fa-3x"></i></mdb-btn>
                            </div>
                            <div class=nav-right>
                                <mdb-btn color="tertiary" title="Next Page" size="sm"><i class="fas fa-arrow-right fa-3x"></i></mdb-btn>
                            </div>
                        </div>
                        <div class="button_grid mt-4" v-bind:key="launcher_items.length">
                            <input v-for="i in 16" v-bind:key="i" v-bind:style="{
                                backgroundColor: (launcher_items[i-1] !== undefined ? launcher_items[i-1].colour: 'gray'),
                                }" class="butt" type="button" style="color:transparent" v-bind:value="i-1" v-on:click="buttonClick"/>
                        </div>
                    </mdb-card-body>
                </mdb-card>
            </mdb-col>
            <mdb-col md="6">
                <mdb-card class="mb-4">
                    <mdb-card-header>Selected app</mdb-card-header>
                    <mdb-card-body>
                        <p v-if="current_app !== undefined">
                            {{ current_app.name }}
                        </p>
                        <p v-else>Click on a keypad button on the left to select</p>
                    </mdb-card-body>
                </mdb-card>
            </mdb-col>
        </mdb-row>
        <mdb-row>
            <mdb-col md="12">
                <mdb-card class="mb-4">
                    <mdb-card-header>Install apps from app store</mdb-card-header>
                    <mdb-card-body>
                        <mdb-row>
                            <mdb-col sm="2">
                                <span>Filter category</span>
                                <select class="browser-default custom-select" v-model="selected_store_category">
                                    <option v-for="category in categories" v-bind:key="category" v-bind:value="category">{{category}}</option>
                                </select>
                            </mdb-col>
                        </mdb-row>
                        <mdb-row class="mt-2 mb-2">
                            <mdb-col sm="2">
                                <span>Filter development state</span>
                                <select class="browser-default custom-select" v-model="selected_store_state">
                                    <option v-for="state in states" v-bind:key="state" v-bind:value="state">{{state}}</option>
                                </select>
                            </mdb-col>
                        </mdb-row>

                        <mdb-tbl class="table-striped">
                            <thead>
                            <tr><th>App name</th><th>Description</th><th>Category</th><th>State</th><th>Author</th><th>Install</th></tr>
                            </thead>
                            <tbody>
                                <tr v-for="app in filtered_store_apps" v-bind:key="app.slug">
                                    <th scope="row">{{ app.name }}</th>
                                    <td>{{ app.description }}</td>
                                    <td>{{ app.category }}</td>
                                    <td>{{ app.status }}</td>
                                    <td>{{ app.author || 'Unknown' }}</td>
                                    <td>
                                        <mdb-btn color="primary" size="sm" v-bind:class="{disabled: installing}" v-on:click="install_app(app.slug)" v-if="local_apps.indexOf(app.slug) === -1">Install</mdb-btn>
                                        <mdb-btn color="gray" size="sm" disabled v-else>Installed</mdb-btn>
                                    </td>
                                </tr>
                            </tbody>
                        </mdb-tbl>
                    </mdb-card-body>
                </mdb-card>
            </mdb-col>
        </mdb-row>
    </section>
</template>

<script>
    import {
        mdbRow,
        mdbCol,
        mdbBtn,
        mdbTbl,
        mdbCard,
        mdbCardBody,
        mdbCardHeader,
    } from 'mdbvue';

    import {on_connect, readfile, createfolder, savefile, fetch_dir} from '../webusb';
    import * as pako from 'pako';
    import * as untar from 'js-untar';
    window.pako = pako;
    window.untar = untar;

    let component = undefined;

    export default {
        name: 'Apps',
        components: {
            mdbRow,
            mdbCol,
            mdbBtn,
            mdbTbl,
            mdbCard,
            mdbCardBody,
            mdbCardHeader,
        },
        beforeMount() {
            component = this;
            on_connect().then(async () => {
                let contents = await readfile('/flash/config/launcher_items.json', );
                component.launcher_items = JSON.parse(contents);
                await component.update_local_apps();
            });

            fetch('https://hatchery.badge.team/basket/campzone2019/list/json',{mode:'cors'})
                .then(response => {response.json().then((apps) => {
                    component.store_apps = apps;
                    for(let app of apps) {
                        if(component.categories.indexOf(app.category) === -1) {
                            component.categories.push(app.category);
                        }
                        if(component.states.indexOf(app.status) === -1) {
                            component.states.push(app.status);
                        }
                    }
                })});
        },
        methods: {
            update_local_apps: async () => {
                let install_paths = ['/flash/apps'];
                let apps = [];

                for(let path of install_paths) {
                    let dir = await fetch_dir(path);
                    let textdecoder = new TextDecoder("ascii");
                    let dir_contents = textdecoder.decode(dir).split('\n');
                    for (let item of dir_contents) {
                        if(item[0] !== 'd') { continue; }
                        apps.push(item.substr(1));
                    }
                }
                component.local_apps = apps;
            },
            get_local_app_metadata: async (app_slug, install_path='/flash/apps/') => {
                let contents = await readfile(install_path + app_slug + '/metadata.json');
                debugger;
                return {
                    ...JSON.parse(contents),
                    slug: app_slug
                };
            },
            get_app_metadata: async (app_slug) => {
                let metadata_url = 'https://badge.team/eggs/get/' + app_slug + '/json';
                let response = await fetch(metadata_url);
                let metadata = await response.json();
                let release_keys = Object.keys(metadata.releases).sort();
                let latest_release_key = release_keys[release_keys.length-1];
                metadata['latest_release_url'] = metadata.releases[latest_release_key][0]['url'];
                return metadata;
            },
            install_app: async (app_slug, install_path='/flash/apps/') => {
                component.installing = true;
                let metadata = await component.get_app_metadata(app_slug);
                let response = await fetch(metadata.latest_release_url);
                let tar_gz = await response.arrayBuffer();
                let tar = pako.inflate(tar_gz);
                let files = await untar(tar.buffer);

                let paths = [];
                for(let file of files) {
                    let dirs = file.name.split('/');
                    dirs.pop();
                    for(let i=1; i <= dirs.length; i++) {
                        paths.push(install_path + dirs.slice(0,i).join('/'));
                    }
                }
                let unique_paths = paths.filter((value, index, self) => self.indexOf(value) === index);
                for(let path of unique_paths) {
                    console.info('Creating folder', path);
                    await createfolder(path);
                }

                for(let file of files) {
                    let path = install_path + file.name;
                    console.info('Writing file', path);
                    await savefile(path, file.buffer);
                }

                component.installing = false;
                component.local_apps.push(app_slug);
                component.$emit('genNotification', 'Installed ' + metadata.name + ' successfully');
            },
            buttonClick: async (event) => {
                debugger;
                let index = parseInt(event.target.value) ;
                let absolute_index = index + component.current_page * 16;
                component.current_index = absolute_index.toString();
                if(!(absolute_index in component.launcher_items)) {
                    component.launcher_items[absolute_index] = {
                        name: '',
                        description: '',
                        category: '',
                        author: '',
                        revision: '',
                    };
                } else {
                    let app_slug = component.launcher_items[absolute_index].slug;
                    component.current_app = await component.get_local_app_metadata(app_slug);
                }
            },
        },
        data() {
            return {
                current_page: 0,
                current_index: -1,
                current_app: undefined,
                launcher_items: {},
                local_apps: [],
                store_apps: [],
                selected_store_category: 'all',
                selected_store_state: 'working',
                categories: ['all'],
                states: ['working', 'all'],
                installing: false
            }
        },
        computed: {
            filtered_store_apps: () => {
                if(component.selected_store_category === 'all' && component.selected_store_state === 'all') {
                    return component.store_apps;
                }
                return component.store_apps.filter((app) =>
                    (component.selected_store_category === 'all' || app.category === component.selected_store_category) &&
                    (component.selected_store_state === 'all' || app.status === component.selected_store_state));
            }
        }
    }
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
    .cascading-admin-card {
        margin: 20px 0;
    }

    .cascading-admin-card .admin-up {
        margin-left: 4%;
        margin-right: 4%;
        margin-top: -20px;
    }

    .cascading-admin-card .admin-up .fas,
    .cascading-admin-card .admin-up .far {
        box-shadow: 0 2px 9px 0 rgba(0, 0, 0, 0.2), 0 2px 13px 0 rgba(0, 0, 0, 0.19);
        padding: 1.7rem;
        font-size: 2rem;
        color: #fff;
        text-align: left;
        margin-right: 1rem;
        border-radius: 3px;
    }

    .cascading-admin-card .admin-up .data {
        float: right;
        margin-top: 2rem;
        text-align: right;
    }

    .admin-up .data p {
        color: #999999;
        font-size: 12px;
    }

    .classic-admin-card .card-body {
        color: #fff;
        margin-bottom: 0;
        padding: 0.9rem;
    }

    .classic-admin-card .card-body p {
        font-size: 13px;
        opacity: 0.7;
        margin-bottom: 0;
    }

    .classic-admin-card .card-body h4 {
        margin-top: 10px;
    }


    .butt {
        border: 0px;
        height:125px;
        width:125px;
        cursor:pointer;
        border-radius: 10px;
        transition: .5s ease;
        backface-visibility: hidden;
        margin: 0 auto;
    }

    .butt:hover {
        opacity: 0.3;
        color:white;
    }

    .button_grid {
        display: grid;
        grid-template-columns: repeat(4, 1fr);
        grid-row-gap: 20px;
        grid-column-gap: 20px;
    }

    .header {
        margin: 10px;
    }

    .nav-left {
        text-align: left;
        float:left; /* add this */
    }
    .nav-right {
        text-align: right;
    }

    .badge_container {
        width: 600px;
        margin: 0 auto;
    }
</style>
