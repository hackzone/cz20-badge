<template>
  <div class="flexible-content">
    <mdb-modal size="lg" style="cursor: pointer; z-index: 9999;" v-show="!is_connected" @click.native="connect()">
      <mdb-modal-header>
        <mdb-modal-title>Let's get started</mdb-modal-title>
      </mdb-modal-header>
      <mdb-modal-body>Click anywhere to connect to your CampZone2020 Event Badge</mdb-modal-body>
    </mdb-modal>
    <!-- Sidebar -->
    <div class="sidebar-fixed position-fixed">
      <a class="logo-wrapper"
        ><img alt="" class="img-fluid" src="./assets/hackzone_logo.png"
      /></a>
      <mdb-list-group class="list-group-flush">
        <router-link to="/apps" @click.native="activeItem = 1">
          <mdb-list-group-item
            :action="true"
            :class="activeItem === 1 && 'active'"
            ><mdb-icon
              icon="table"
              class="mr-3"
            />Apps</mdb-list-group-item
          >
        </router-link>
        <router-link to="/programming" @click.native="activeItem = 2">
          <mdb-list-group-item
            :action="true"
            :class="activeItem === 2 && 'active'"
            ><mdb-icon icon="i-cursor" class="mr-3" />Programming & Files</mdb-list-group-item
          >
        </router-link>
      </mdb-list-group>
    </div>
    <!-- /Sidebar  -->
    <main>
      <div class="p-5">
        <router-view></router-view>
      </div>
      <ftr color="primary-color-dark" class="text-center font-small darken-2">
        <div class="pb-4 pt-4">
          <a href="#"><mdb-icon fab icon="facebook-square" class="mr-3"/></a>
          <a href="#"><mdb-icon fab icon="twitter" class="mr-3"/></a>
          <a href="#"><mdb-icon fab icon="youtube" class="mr-3"/></a>
          <a href="#"><mdb-icon fab icon="google-plus" class="mr-3"/></a>
          <a href="#"><mdb-icon fab icon="dribbble" class="mr-3"/></a>
          <a href="#"><mdb-icon fab icon="pinterest" class="mr-3"/></a>
          <a href="#"><mdb-icon fab icon="github" class="mr-3"/></a>
          <a href="#"><mdb-icon fab icon="codepen" class="mr-3"/></a>
        </div>
      </ftr>
    </main>
  </div>
</template>

<script>
import {
  mdbModal,
  mdbModalHeader,
  mdbModalTitle,
  mdbModalBody,
  mdbIcon,
  mdbListGroup,
  mdbListGroupItem,
  mdbFooter,
  waves
} from "mdbvue";

import {device, connect} from './webusb';

let component = undefined;
setInterval(() => {
  component.is_connected = device !== undefined && device.opened;
}, 1000);

export default {
  name: "AdminTemplate",
  components: {
    mdbModal,
    mdbModalHeader,
    mdbModalTitle,
    mdbModalBody,
    mdbListGroup,
    mdbListGroupItem,
    mdbIcon,
    ftr: mdbFooter
  },
  methods: {
    on_click: () => {console.log('clicked')},
    connect:connect,
  },
  data() {
    return {
      activeItem: 1,
      is_connected: device !== undefined && device.opened
    };
  },
  beforeMount() {
    component = this;
    this.activeItem = this.$route.matched[0].props.default.page;
  },
  mixins: [waves]
};
</script>


<style>
@import url("https://fonts.googleapis.com/css?family=Roboto:300,400,500,700&display=swap");
.navbar-light .navbar-brand {
  margin-left: 15px;
  color: #2196f3 !important;
  font-weight: bolder;
}
</style>

<style scoped>
main {
  background-color: #ededee;
}

.flexible-content {
  transition: padding-left 0.3s;
  padding-left: 270px;
}

.flexible-navbar {
  transition: padding-left 0.5s;
  padding-left: 270px;
}

.sidebar-fixed {
  left: 0;
  top: 0;
  height: 100vh;
  width: 270px;
  box-shadow: 0 2px 5px 0 rgba(0, 0, 0, 0.16), 0 2px 10px 0 rgba(0, 0, 0, 0.12);
  z-index: 1050;
  background-color: #fff;
  padding: 1.5rem;
  padding-top: 0;
}

.sidebar-fixed .logo-wrapper img {
  width: 100%;
  padding: 2.5rem;
}

.sidebar-fixed .list-group-item {
  display: block !important;
  transition: background-color 0.3s;
}

.sidebar-fixed .list-group .active {
  box-shadow: 0 2px 5px 0 rgba(0, 0, 0, 0.16), 0 2px 10px 0 rgba(0, 0, 0, 0.12);
  border-radius: 5px;
}
</style>
