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
                        <div class="button_grid mt-4">
                            <input v-for="i in 16" v-bind:key="i" class="butt" type="button" value="test"/>
                        </div>
                    </mdb-card-body>
                </mdb-card>
            </mdb-col>
            <mdb-col md="6">
                <mdb-card class="mb-4">
                    <mdb-card-header>Selected app</mdb-card-header>
                    <mdb-card-body>
                        <p v-if="currentApp !== undefined">

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
                        noot noot
                        <hr>
                        noot too
                        {{launcher_items}}
                    </mdb-card-body>
                </mdb-card>
            </mdb-col>
        </mdb-row>
        <section>
            <mdb-row>
                <mdb-col lg="12" class="mb-4">
                    <mdb-card>
                        <mdb-card-header>Modals</mdb-card-header>
                        <mdb-card-body>
                            <section>
                                <h4 class="mb-5 mt-4 dark-grey-text text-center font-bold"><strong>Position &
                                    Sizes</strong></h4>
                                <div class="text-center mb-5">
                                    <p class="lead">Click buttons below to launch modals demo</p>
                                </div>
                                <mdb-row>
                                    <mdb-col md="3" class="mb-3">
                                        <h5 class="text-center mb-3">Frame modal</h5>
                                        <img class="img-fluid z-depth-1"
                                             src="https://mdbootstrap.com/img/brandflow/modal4.jpg"
                                             alt="frame position"/>
                                        <div class="text-center">
                                            <h5 class="my-3">Position</h5>
                                            <mdb-btn color="primary" size="sm" @click.native="showFrameModalTop = true">
                                                Top
                                            </mdb-btn>
                                            <mdb-modal frame removeBackdrop position="top" :show="showFrameModalTop"
                                                       @close="showFrameModalTop = false">
                                                <mdb-modal-body class="text-center">
                                                    <span>Lorem ipsum dolor sit amet, consectetur adipisicing elit. Impedit nisi quo provident fugiat reprehenderit nostrum quos...</span>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showFrameModalTop = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-body>
                                            </mdb-modal>
                                            <mdb-btn color="primary" size="sm"
                                                     @click.native="showFrameModalBottom = true">Bottom
                                            </mdb-btn>
                                            <mdb-modal frame removeBackdrop position="bottom"
                                                       :show="showFrameModalBottom"
                                                       @close="showFrameModalBottom = false">
                                                <mdb-modal-body class="text-center">
                                                    <span>Lorem ipsum dolor sit amet, consectetur adipisicing elit. Impedit nisi quo provident fugiat reprehenderit nostrum quos...</span>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showFrameModalBottom = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-body>
                                            </mdb-modal>
                                        </div>
                                    </mdb-col>
                                    <mdb-col md="3" class="mb-3">
                                        <h5 class="text-center mb-3">Side modal</h5>
                                        <img class="img-fluid z-depth-1"
                                             src="https://mdbootstrap.com/img/brandflow/modal3.jpg"
                                             alt="frame position"/>
                                        <div class="text-center">
                                            <h5 class="my-3">Position</h5>
                                            <mdb-btn color="primary" size="sm"
                                                     @click.native="showSideModalTopRight = true">Top right
                                            </mdb-btn>
                                            <mdb-modal side position="top-right" :show="showSideModalTopRight"
                                                       @close="showSideModalTopRight = false">
                                                <mdb-modal-header>
                                                    <mdb-modal-title>Modal title</mdb-modal-title>
                                                </mdb-modal-header>
                                                <mdb-modal-body>...</mdb-modal-body>
                                                <mdb-modal-footer>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showSideModalTopRight = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-footer>
                                            </mdb-modal>
                                            <mdb-btn color="primary" size="sm"
                                                     @click.native="showSideModalTopLeft = true">Top left
                                            </mdb-btn>
                                            <mdb-modal side position="top-left" :show="showSideModalTopLeft"
                                                       @close="showSideModalTopLeft = false">
                                                <mdb-modal-header>
                                                    <mdb-modal-title>Modal title</mdb-modal-title>
                                                </mdb-modal-header>
                                                <mdb-modal-body>...</mdb-modal-body>
                                                <mdb-modal-footer>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showSideModalTopLeft = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-footer>
                                            </mdb-modal>
                                            <mdb-btn color="primary" size="sm"
                                                     @click.native="showSideModalBottomRight = true">Bottom right
                                            </mdb-btn>
                                            <mdb-modal side position="bottom-right" :show="showSideModalBottomRight"
                                                       @close="showSideModalBottomRight = false">
                                                <mdb-modal-header>
                                                    <mdb-modal-title>Modal title</mdb-modal-title>
                                                </mdb-modal-header>
                                                <mdb-modal-body>...</mdb-modal-body>
                                                <mdb-modal-footer>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showSideModalBottomRight = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-footer>
                                            </mdb-modal>
                                            <mdb-btn color="primary" size="sm"
                                                     @click.native="showSideModalBottomLeft = true">Bottom left
                                            </mdb-btn>
                                            <mdb-modal side position="bottom-left" :show="showSideModalBottomLeft"
                                                       @close="showSideModalBottomLeft = false">
                                                <mdb-modal-header>
                                                    <mdb-modal-title>Modal title</mdb-modal-title>
                                                </mdb-modal-header>
                                                <mdb-modal-body>...</mdb-modal-body>
                                                <mdb-modal-footer>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showSideModalBottomLeft = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-footer>
                                            </mdb-modal>
                                        </div>
                                    </mdb-col>
                                    <mdb-col md="3" class="mb-3">
                                        <h5 class="text-center mb-3">Central modal</h5>
                                        <img class="img-fluid z-depth-1"
                                             src="https://mdbootstrap.com/img/brandflow/modal2.jpg"
                                             alt="frame position"/>
                                        <div class="text-center">
                                            <h5 class="my-3">Size</h5>
                                            <mdb-btn color="primary" size="sm"
                                                     @click.native="showCentralModalSmall = true">Small
                                            </mdb-btn>
                                            <mdb-modal size="sm" :show="showCentralModalSmall"
                                                       @close="showCentralModalSmall = false">
                                                <mdb-modal-header>
                                                    <mdb-modal-title>Modal title</mdb-modal-title>
                                                </mdb-modal-header>
                                                <mdb-modal-body>...</mdb-modal-body>
                                                <mdb-modal-footer>
                                                    <mdb-btn size="sm" color="secondary"
                                                             @click.native="showCentralModalSmall = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn size="sm" color="primary">Save changes</mdb-btn>
                                                </mdb-modal-footer>
                                            </mdb-modal>
                                            <mdb-btn color="primary" size="sm"
                                                     @click.native="showCentralModalMedium = true">Medium
                                            </mdb-btn>
                                            <mdb-modal :show="showCentralModalMedium"
                                                       @close="showCentralModalMedium = false">
                                                <mdb-modal-header>
                                                    <mdb-modal-title>Modal title</mdb-modal-title>
                                                </mdb-modal-header>
                                                <mdb-modal-body>...</mdb-modal-body>
                                                <mdb-modal-footer>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showCentralModalMedium = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-footer>
                                            </mdb-modal>
                                            <mdb-btn color="primary" size="sm"
                                                     @click.native="showCentralModalLarge = true">Large
                                            </mdb-btn>
                                            <mdb-modal size="lg" :show="showCentralModalLarge"
                                                       @close="showCentralModalLarge = false">
                                                <mdb-modal-header>
                                                    <mdb-modal-title>Modal title</mdb-modal-title>
                                                </mdb-modal-header>
                                                <mdb-modal-body>...</mdb-modal-body>
                                                <mdb-modal-footer>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showCentralModalLarge = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-footer>
                                            </mdb-modal>
                                            <mdb-btn color="primary" size="sm"
                                                     @click.native="showCentralModalFluid = true">Fluid
                                            </mdb-btn>
                                            <mdb-modal size="fluid" :show="showCentralModalFluid"
                                                       @close="showCentralModalFluid = false">
                                                <mdb-modal-header>
                                                    <mdb-modal-title>Modal title</mdb-modal-title>
                                                </mdb-modal-header>
                                                <mdb-modal-body>...</mdb-modal-body>
                                                <mdb-modal-footer>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showCentralModalFluid = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-footer>
                                            </mdb-modal>
                                        </div>
                                    </mdb-col>
                                    <mdb-col md="3" class="mb-3">
                                        <h5 class="text-center mb-3">Fluid modal</h5>
                                        <img class="img-fluid z-depth-1"
                                             src="https://mdbootstrap.com/img/brandflow/modal1.jpg"
                                             alt="frame position"/>
                                        <div class="text-center">
                                            <h5 class="my-3">Position</h5>
                                            <mdb-btn color="primary" size="sm"
                                                     @click.native="showFluidModalRight = true">Right
                                            </mdb-btn>
                                            <mdb-modal fullHeight position="right" :show="showFluidModalRight"
                                                       @close="showFluidModalRight = false">
                                                <mdb-modal-header>
                                                    <mdb-modal-title>Modal title</mdb-modal-title>
                                                </mdb-modal-header>
                                                <mdb-modal-body>...</mdb-modal-body>
                                                <mdb-modal-footer>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showFluidModalRight = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-footer>
                                            </mdb-modal>
                                            <mdb-btn color="primary" size="sm"
                                                     @click.native="showFluidModalLeft = true">Left
                                            </mdb-btn>
                                            <mdb-modal fullHeight position="left" :show="showFluidModalLeft"
                                                       @close="showFluidModalLeft = false">
                                                <mdb-modal-header>
                                                    <mdb-modal-title>Modal title</mdb-modal-title>
                                                </mdb-modal-header>
                                                <mdb-modal-body>...</mdb-modal-body>
                                                <mdb-modal-footer>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showFluidModalLeft = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-footer>
                                            </mdb-modal>
                                            <mdb-btn color="primary" size="sm" @click.native="showFluidModalTop = true">
                                                Top
                                            </mdb-btn>
                                            <mdb-modal fullHeight position="top" :show="showFluidModalTop"
                                                       @close="showFluidModalTop = false">
                                                <mdb-modal-header>
                                                    <mdb-modal-title>Modal title</mdb-modal-title>
                                                </mdb-modal-header>
                                                <mdb-modal-body>...</mdb-modal-body>
                                                <mdb-modal-footer>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showFluidModalTop = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-footer>
                                            </mdb-modal>
                                            <mdb-btn color="primary" size="sm"
                                                     @click.native="showFluidModalBottom = true">Bottom
                                            </mdb-btn>
                                            <mdb-modal fullHeight position="bottom" :show="showFluidModalBottom"
                                                       @close="showFluidModalBottom = false">
                                                <mdb-modal-header>
                                                    <mdb-modal-title>Modal title</mdb-modal-title>
                                                </mdb-modal-header>
                                                <mdb-modal-body>...</mdb-modal-body>
                                                <mdb-modal-footer>
                                                    <mdb-btn color="secondary"
                                                             @click.native="showFluidModalBottom = false">Close
                                                    </mdb-btn>
                                                    <mdb-btn color="primary">Save changes</mdb-btn>
                                                </mdb-modal-footer>
                                            </mdb-modal>
                                        </div>
                                    </mdb-col>
                                </mdb-row>
                            </section>
                        </mdb-card-body>
                    </mdb-card>
                </mdb-col>
            </mdb-row>
        </section>
    </section>
</template>

<script>
    import {
        mdbRow,
        mdbCol,
        mdbBtn,
        mdbCard,
        mdbCardBody,
        mdbCardHeader,
        mdbModal,
        mdbModalHeader,
        mdbModalTitle,
        mdbModalBody,
        mdbModalFooter
    } from 'mdbvue';

    import {on_connect, readfile} from '../webusb';

    let component = undefined;

    export default {
        name: 'Dashboard',
        components: {
            mdbRow,
            mdbCol,
            mdbBtn,
            mdbCard,
            mdbCardBody,
            mdbCardHeader,
            mdbModal,
            mdbModalHeader,
            mdbModalTitle,
            mdbModalBody,
            mdbModalFooter,
        },
        beforeMount() {
            component = this;
            on_connect().then(() => {
                readfile('/flash/config/launcher_items.json', (contents) => {
                    component.launcher_items = JSON.parse(contents);
                });
            });
        },
        data() {
            return {
                showFrameModalTop: false,
                showFrameModalBottom: false,
                showSideModalTopRight: false,
                showSideModalTopLeft: false,
                showSideModalBottomRight: false,
                showSideModalBottomLeft: false,
                showCentralModalSmall: false,
                showCentralModalMedium: false,
                showCentralModalLarge: false,
                showCentralModalFluid: false,
                showFluidModalRight: false,
                showFluidModalLeft: false,
                showFluidModalTop: false,
                showFluidModalBottom: false,
                currentApp:{
                    name: '',
                    description: '',
                    category: '',
                    author: '',
                    revision: '',
                },
                launcher_items: {}
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
        background-color: rgb(19, 122, 156);
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


    /* Style buttons */
    .btn {
        background-color: #15171B; /* Blue background */
        border: none; /* Remove borders */
        color: white; /* White text */
        padding: 12px 16px; /* Some padding */
        font-size: 14px; /* Set a font size */
        cursor: pointer; /* Mouse pointer on hover */
    }

    /* Darker background on mouse-over */
    .btn:hover {
        background-color: RoyalBlue;
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
