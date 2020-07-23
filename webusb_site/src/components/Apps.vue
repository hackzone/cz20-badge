<template>
    <section id="apps">
        <mdb-modal size="lg" style="cursor: pointer; z-index: 9999;" v-if="installing">
            <mdb-modal-body>
                Currently writing: {{ installing_file }}
                <mdb-progress :height="20" v-model="installing_progress" />
            </mdb-modal-body>
        </mdb-modal>
        <mdb-row>
            <mdb-col md="5">
                <mdb-card class="mb-4">
                    <mdb-card-body class="badge_container">
                        <mdb-row>
                            <mdb-col sm="4" class=nav-left>
                                <mdb-btn color="primary" title="Previous Page" size="sm" v-on:click="pagedec"><i class="fas fa-arrow-left fa-3x"></i></mdb-btn>
                            </mdb-col>
                            <mdb-col sm="4" class=page>
                            <H1>{{current_page}}</H1>
                            </mdb-col>
                            <mdb-col sm="4" class=nav-right>                           
                                <mdb-btn color="tertiary" title="Next Page" size="sm" v-on:click="pageinc"><i class="fas fa-arrow-right fa-3x"></i></mdb-btn>
                            </mdb-col>
                        </mdb-row>
                        <mdb-row class="button_grid mt-4 mb-4" v-bind:key="launcher_items.length">

                                <div v-for="i in 16" v-bind:key="i" v-bind:style="{
                                    backgroundColor: ((launcher_items[(i-1)+(current_page*16)] !== undefined &&
                                    launcher_items[(i-1)+(current_page*16)].colour !== undefined) ?
                                    launcher_items[(i-1)+(current_page*16)].colour : 'gray'),
                                    filter: (i-1 === current_index) ? 'drop-shadow(gray 0px 0px 5px)': ''
                                    }" class="butt" type="button" style="color:transparent" v-bind:id="(i-1)+(current_page*16)" v-on:click="buttonClick"></div>

                        </mdb-row>
                    </mdb-card-body>
                </mdb-card>
            </mdb-col>
            <mdb-col md="6">
                <mdb-card class="mb-4">
                    <mdb-card-header>Configure home screen buttons</mdb-card-header>
                    <mdb-card-body v-if="current_index >= 0">
                        <span>Currently configured app</span>
                        <select class="browser-default custom-select" v-model="current_app_slug" v-on:change="update_current_launcher_item">
                            <option value="none"><b>No app</b></option>
                            <option v-for="app_name in local_apps" v-bind:key="app_name" v-bind:value="app_name">{{app_name}}</option>
                        </select>
                        <div v-if="current_app !== undefined" class="mt-3">
                            <p><strong>{{ current_app.name }}</strong></p>
                            <vue-markdown v-bind:source="current_app.description"></vue-markdown>
                            <p v-if="store_apps.filter((val) => val.name === current_app.name).length == 1 && parseInt(store_apps.filter((val) => val.name === current_app.name)[0].revision) !== current_app.revision">An update is available for this app
                            <mdb-btn color="primary" size="sm" v-if="parseInt(store_apps.filter((val) => val.name === current_app.name)[0].revision) !== current_app.revision" v-bind:class="{disabled: installing}" v-on:click="install_app(current_app.slug, is_update=true)">Install</mdb-btn>
                            </p>
                        </div>

                        <sketch-picker v-model="color_picker" @input="update_current_colour" />

                    </mdb-card-body>
                    <mdb-card-body v-else>
                        <span>Here you can connect apps to buttons on the homescreen you see when your badge starts up. Select a button to configure on the left.</span>
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
                            <mdb-col sm="6" md="3">
                                <span>Filter category</span>
                                <select class="browser-default custom-select" v-model="selected_store_category">
                                    <option v-for="category in categories" v-bind:key="category" v-bind:value="category">{{category}}</option>
                                </select>
                            </mdb-col>
                            <mdb-col sm="6" md="3">
                                <span>Filter development state</span>
                                <select class="browser-default custom-select" v-model="selected_store_state">
                                    <option v-for="state in states" v-bind:key="state" v-bind:value="state">{{state}}</option>
                                </select>
                            </mdb-col>
                            <mdb-col sm="6">
                                <span>Apps uploaded to hatchery.badge.team will automatically show up here for everyone. Be sure to upload yours! :)</span>
                            </mdb-col>
                        </mdb-row>

                        <mdb-tbl class="table-striped">
                            <thead>
                            <tr><th>App name</th><th>Description</th><th>Category</th><th>State</th><th>Author</th><th>Revision</th><th>Install</th></tr>
                            </thead>
                            <tbody>
                                <tr v-for="app in filtered_store_apps" v-bind:key="app.slug">
                                    <th scope="row">{{ app.name }}</th>
                                    <td>{{ app.description }}</td>
                                    <td>{{ app.category }}</td>
                                    <td>{{ app.status }}</td>
                                    <td>{{ app.author || 'Unknown' }}</td>
                                    <td>{{ app.revision }}</td>
                                    <td>
                                        <mdb-btn color="primary" size="sm" v-bind:class="{disabled: installing}" v-on:click="install_app(app.slug)" v-if="local_apps.indexOf(app.slug) === -1">Install</mdb-btn>
                                        <mdb-btn color="red" size="sm" v-on:click="uninstall_app(app.slug)" v-else>Uninstall</mdb-btn>
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
        mdbProgress,
        mdbModal,
        mdbModalBody,
        mdbCard,
        mdbCardBody,
        mdbCardHeader,
    } from 'mdbvue';

    import VueMarkdown from 'vue-markdown';
    import {Sketch} from 'vue-color';
    import {on_connect, readfile, createfolder, savefile, fetch_dir, deldir, savetextfile} from '../webusb';
    import * as pako from 'pako';
    import * as untar from 'js-untar';
    window.pako = pako;
    window.untar = untar;
    window.VueMarkdown = VueMarkdown;

    let component = undefined;
    debugger;

    export default {
        name: 'Apps',
        components: {
            mdbRow,
            mdbCol,
            mdbBtn,
            mdbTbl,
            mdbProgress,
            mdbModal,
            mdbModalBody,
            mdbCard,
            mdbCardBody,
            mdbCardHeader,
            'vue-markdown': VueMarkdown,
            'sketch-picker': Sketch,
        },
        beforeMount() {
            component = this;
            on_connect().then(async () => {
                let contents = await readfile('/flash/config/system-launcher_items.json', );
                let launcher_items;
                try {
                    launcher_items = JSON.parse(contents);
                } catch {
                    launcher_items = {};
                }
                component.launcher_items = launcher_items;
                await component.update_local_apps();
            });

            fetch('https://hatchery.badge.team/basket/campzone2020/list/json',{mode:'cors'})
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
                    let dir_contents = dir.split('\n');
                    for (let item of dir_contents) {
                        if(item[0] !== 'd') { continue; }
                        apps.push(item.substr(1));
                    }
                }
                component.local_apps = apps;
            },
            get_local_app_metadata: async (app_slug, install_path='/flash/apps/') => {
                let contents = await readfile(install_path + app_slug + '/metadata.json');
                let metadata;
                try {
                    metadata = JSON.parse(contents);
                } catch {
                    metadata = {};
                }
                return {
                    ...metadata,
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
            install_app: async (app_slug, is_update=false, install_path='/flash/apps/') => {
                component.installing_file = '';
                component.installing_progress = 0;
                component.installing = true;
                try {
                    let metadata = await component.get_app_metadata(app_slug);
                    let response = await fetch(metadata.latest_release_url);
                    let tar_gz = await response.arrayBuffer();
                    let tar = pako.inflate(tar_gz);
                    let files = await untar(tar.buffer);

                    let paths = [];
                    for (let file of files) {
                        let dirs = file.name.split('/');
                        dirs.pop();
                        for (let i = 1; i <= dirs.length; i++) {
                            paths.push(install_path + dirs.slice(0, i).join('/'));
                        }
                    }

                    if (!is_update) {
                        let unique_paths = paths.filter((value, index, self) => self.indexOf(value) === index);
                        for (let path of unique_paths) {
                            console.info('Creating folder', path);
                            await createfolder(path);
                        }
                    }

                    for (let [i, file] of files.entries()) {
                        let path = install_path + file.name;
                        console.info('Writing file', path);
                        component.installing_file = path;
                        component.installing_progress = Math.floor((i / (files.length-1)) * 100);
                        await savefile(path, file.buffer);
                    }

                    if (is_update) {
                        component.current_app = await component.get_local_app_metadata(app_slug, install_path);
                    } else {
                        component.local_apps.push(app_slug);
                    }
                    component.$emit('genNotification', 'Installed ' + metadata.name + ' successfully');
                } catch(error) {
                    try {
                        await deldir(install_path + app_slug);
                    } catch(error) {
                        console.error('Error removing app dir for ' + app_slug, error);
                    }
                    component.$emit('genNotification', 'Failed to install ' + app_slug + '. Please try again.');
                } finally {
                    component.installing = false;
                }
            },
            uninstall_app: async (app_slug, install_path='/flash/apps/') => {
                try {
                    await deldir(install_path+app_slug);
                    component.$emit('genNotification', 'Uninstalled ' + app_slug + ' successfully');
                } catch {
                    component.$emit('genNotification', 'Uninstalled ' + app_slug + ' not successfully', 'exclamation');
                }
                component.update_local_apps();
            },
            buttonClick: async (event) => {
                let index = parseInt(event.target.id) ;
                component.current_index = index;
                if(!(index in component.launcher_items)) {
                    component.current_app = undefined;
                    component.current_app_slug = 'none';
                } else {
                    let launcher_item = component.launcher_items[index.toString()];
                    component.color_picker = component.current_colour = launcher_item.colour;
                    component.current_app_slug = launcher_item.slug;
                    component.current_app = await component.get_local_app_metadata(launcher_item.slug);
                }
            },
            update_current_colour: (colour) => {
                clearTimeout(component.timeout_colorpicker);
                component.timeout_colorpicker = setTimeout(() => {
                    component.current_colour = colour.hex;
                    component.update_current_launcher_item();
                }, 800);
            },
            update_current_launcher_item: async () => {
                let index = component.current_index;
                let slug = component.current_app_slug;
                if (slug === 'none') {
                    delete component.launcher_items[index.toString()];
                } else {
                    let app = await component.get_local_app_metadata(slug);
                    component.current_app = app;
                    component.launcher_items[index.toString()] = {
                        slug,
                        name: app.name,
                        colour: component.current_colour,
                    };
                }
                await savetextfile('/flash/config/system-launcher_items.json', JSON.stringify(component.launcher_items));
                component.$emit('genNotification', 'Updated homescreen');
            },
            pageinc: () => {
                component.current_page += 1;
            },
            pagedec: () => {
                if(component.current_page > 0) {
                     component.current_page -= 1;
                }
            }
        },
        data() {
            return {
                timeout_colorpicker: null,
                current_page: 0,
                current_index: -1,
                current_app: undefined,
                current_app_slug: undefined,
                current_colour: '#4A90E2',
                launcher_items: {},
                local_apps: [],
                store_apps: [],
                selected_store_category: 'all',
                selected_store_state: 'working',
                categories: ['all'],
                states: ['working', 'all'],
                installing: false,
                installing_file: '',
                installing_progress: 0,
                color_picker: '007F7F'
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
        content: "";
        border: 0px;
        padding-bottom: 100%;
        display: block;
        width:100%;
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
        grid-row-gap: 3.5%;
        grid-column-gap: 3.5%;
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
        width: 80%;
        display: block;
        padding-bottom: 15%;
        margin: 0 auto;
    }

    .page {
        display: flex;
        justify-content: center;
        align-items: center;
    }
</style>
