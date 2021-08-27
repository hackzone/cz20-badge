<template>
    <section id="apps">
        <mdb-row>
            <mdb-col md="12">
                <mdb-card class="mb-4">
                    <mdb-card-header>Update your badge firmware</mdb-card-header>
                    <mdb-card-body>
                        Your badge is running firmware version: {{checking_badge ? 'Checking...' : badge_firmware_version}}<br><br>

                        Latest available on the server: {{checking_server ? 'Checking...' : server_firmware_version}}<br><br>

                        Make sure you configure your WiFi on the Settings page first. Pressing the update button will reboot the badge. If WiFi can't connect, the LEDs will remain off. Update progress is shown with green LEDs. It's safe to disconnect the badge before and during updating, but try to do that only if the screen remains off for 30+ seconds.<br><br>

                        <mdb-btn color="primary" size="sm" v-on:click="update">Update over WiFi</mdb-btn>
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

    import {Sketch} from 'vue-color';
    import {
        on_connect,
        readfile,
        createfolder,
        savefile,
        fetch_dir,
        deldir,
        savetextfile,
        writetostdin
    } from '../webusb';

    let component = undefined;

    export default {
        name: 'Update',
        components: {
            mdbRow,
            mdbCol,
            mdbBtn,
            mdbCard,
            mdbCardBody,
            mdbCardHeader,
        },
        beforeMount() {
            component = this;
            on_connect().then(async () => {
                let contents = await readfile('/flash/cache/version.txt');
                let version;
                try {
                    version = JSON.parse(contents);
                } catch {
                    version = {'firmware': 'unknown'};
                }
                component.badge_firmware_version = version.firmware;
                component.checking_badge = false;
            });

            fetch('https://raw.githubusercontent.com/badgeteam/ota/master/version/campzone2020.txt',{mode:'cors'})
                .then(response => {response.json().then((version) => {
                    component.server_firmware_version = version.build;
                    component.server_firmware_name = version.name;
                    component.checking_server = false;
                })});
        },
        methods: {
            update: async (event) => {
                writetostdin('system.ota()\r\n');
            }
        },
        data() {
            return {
                badge_firmware_version: 0,
                badge_firmware_name: '',
                server_firmware_version: 0,
                server_firmware_name: '',
                checking_badge: true,
                checking_server: true
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
