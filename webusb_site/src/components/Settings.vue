<template>
    <section id="settings">
        <mdb-row>
            <mdb-col sm="12" xl="6">
                <mdb-card class="mt-4">
                    <mdb-card-header>WiFi Settings</mdb-card-header>
                    <mdb-card-body>
                        <form>
                            <div class="grey-text">
                                <mdb-input v-model="wifi_ssid" label="Access Point name (SSID)" icon="wifi" type="email"/>
                                <mdb-input v-model="wifi_password" label="Password" icon="lock" type="password"/>
                            </div>
                        </form>
                        <mdb-btn color="primary" v-on:click="save_wifi">Save</mdb-btn>
                    </mdb-card-body>
                </mdb-card>
            </mdb-col>

            <mdb-col sm="12" xl="6">
                <mdb-card class="mt-4">
                    <mdb-card-header>Audio Settings</mdb-card-header>
                    <mdb-card-body>
                        <div class="grey-text">
                            <p>Volume: {{volume}}</p>
                            <input type="range" min="0" max="255" class="custom-range" v-model="volume">
                        </div>
                        <mdb-btn color="primary" v-on:click="save_audio">Save</mdb-btn>
                    </mdb-card-body>
                </mdb-card>
            </mdb-col>

            <mdb-col sm="12" xl="6" v-for="app_slug in app_slugs" v-bind:key="app_slug">
                <mdb-card class="mt-4">
                    <mdb-card-header>App - {{app_slug}}</mdb-card-header>
                    <mdb-card-body>
                        <vue-json-editor v-model="configs[app_slug]" :show-btns="false" :expandedOnStart="true" @json-change="onJsonChange"></vue-json-editor>
                        <mdb-btn color="primary" v-on:click="save_app(app_slug)">Save</mdb-btn>
                    </mdb-card-body>
                </mdb-card>
            </mdb-col>
        </mdb-row>
    </section>
</template>

<script>
    import {savetextfile, runfile, readfile, delfile, fetch_dir, on_connect, writetostdin} from '../webusb';
    import {mdbRow, mdbCol, mdbBtn, mdbInput, mdbCard, mdbCardHeader, mdbCardBody} from 'mdbvue';
    import vueJsonEditor from 'vue-json-editor'

    let component;
    export default {
        name: 'Settings',
        beforeMount() {
            component = this;
            on_connect().then(() => this.update_local_apps());
        },
        components: {
            mdbRow,
            mdbCol,
            mdbBtn,
            mdbInput,
            mdbCard,
            mdbCardHeader,
            mdbCardBody,
            vueJsonEditor
        },
        methods: {
            update_local_apps: async () => {
                let install_paths = ['/flash/apps'];
                let configurable_apps = [];

                for(let path of install_paths) {
                    let dir = await fetch_dir(path);
                    let dir_contents = dir.split('\n');
                    for (let item of dir_contents) {
                        if(item[0] !== 'd') { continue; }
                        let app_slug = item.substr(1);
                        let contents;
                        try {
                            contents = await readfile('/flash/config/app-' + app_slug + '.json');
                            if(contents !== undefined && contents.length)
                            {
                                configurable_apps.push(app_slug);
                                try {
                                    component.configs[app_slug] = JSON.parse(contents);
                                } catch {
                                    component.configs[app_slug] = {}
                                }
                            }
                        } catch { let do_nothing = 1; do_nothing++; }
                    }
                }

                component.app_slugs = configurable_apps;
            },
            save_wifi: async () => {
                await writetostdin('import machine, system;'+
                    'machine.nvs_setstr("system", "wifi.ssid", "' + component.wifi_ssid + '");' +
                    'machine.nvs_setstr("system", "wifi.password", "' + component.wifi_password + '");\r\n');
                component.$emit('genNotification', 'WiFi settings updated successfully');
            },
            save_audio: async () => {
                await writetostdin('import machine, system;'+
                    'machine.nvs_setint("system", "volume", ' + component.volume + ')\r\n');
                component.$emit('genNotification', 'Audio settings updated successfully');
            },
            save_app: async (app_slug) => {
                await savetextfile('/flash/config/app-'+app_slug+'.json', JSON.stringify(component.configs[app_slug]));
                component.$emit('genNotification', 'App settings updated successfully');
            },
            onJsonChange: () => {
                debugger;
            }
        },
        data() {
            return {
                wifi_ssid: '',
                wifi_password: '',
                app_slugs: [],
                configs: {},
                volume: 255
            }
        }
    }
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
    .full {
        height: 70vh;
    }
</style>
