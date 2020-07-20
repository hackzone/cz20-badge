<template>
  <div class="flexible-content">
    <mdb-modal size="lg" style="cursor: pointer; z-index: 9999;" v-show="!is_connected" @click.native="connect()">
      <mdb-modal-header>
        <mdb-modal-title>Let's get started</mdb-modal-title>
      </mdb-modal-header>
      <mdb-modal-body>Click anywhere to connect to your CampZone2020 Event Badge via USB</mdb-modal-body>
    </mdb-modal>
    <!-- Sidebar -->
    <div class="sidebar-fixed position-fixed">
      <a class="logo-wrapper"><img alt="" class="img-fluid" src="./assets/hackzone_logo.png"/></a>
      <mdb-list-group class="list-group-flush">
        <router-link to="/apps" @click.native="activeItem = 1">
          <mdb-list-group-item
            :action="true"
            :class="activeItem === 1 && 'active'"
            ><mdb-icon
              icon="table"
              class="mr-3"
            />Apps</mdb-list-group-item>
        </router-link>
        <router-link to="/programming" @click.native="activeItem = 2">
          <mdb-list-group-item
                  :action="true"
                  :class="activeItem === 2 && 'active'"
          ><mdb-icon icon="i-cursor" class="mr-3" style="width:16px;text-align:center" />Programming & Files</mdb-list-group-item>
        </router-link>
        <router-link to="/update" @click.native="activeItem = 3">
          <mdb-list-group-item
                  :action="true"
                  :class="activeItem === 3 && 'active'"
          ><mdb-icon icon="bolt" class="mr-3" style="width:16px;text-align:center" />Update</mdb-list-group-item>
        </router-link>
        <router-link to="/settings" @click.native="activeItem = 4">
          <mdb-list-group-item
                  :action="true"
                  :class="activeItem === 4 && 'active'"
          ><mdb-icon icon="cog" class="mr-3" />Settings</mdb-list-group-item>
        </router-link>
      </mdb-list-group>
    </div>
    <!-- notifications -->
    <div class="container">
        <div class="placement">
          <mdb-toast-notification v-for="message in messages" :key="message.id" :title="message.title" :message="message.message" :icon="message.icon" :iconColor="message.color" :show="show"/>
        </div>
      </div>
    <!-- /Sidebar  -->
    <main>
      <div class="p-5">
        <router-view v-on:genNotification="onNotification">
        </router-view>
      </div>
      <ftr color="primary-color-dark" class="text-center font-small darken-2">
        <div class="pb-4 pt-4">
          <a href="https://twitter.com/hackzonenl"><mdb-icon fab icon="twitter" class="mr-3"/></a>
          <a href="https://www.youtube.com/watch?v=G0mEW8f2vac"><mdb-icon fab icon="youtube" class="mr-3"/></a>
          <a href="https://github.com/hackzone"><mdb-icon fab icon="github" class="mr-3"/></a>
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
  mdbToastNotification,
  waves
} from "mdbvue";

import {device, connect} from './webusb';

let component = undefined;
setInterval(() => {
  component.is_connected = device !== undefined && device.opened;
}, 1000);

setInterval(() => {
  let delete_list = [];
  for(let i=0; i < component.messages.length; i++) {
    component.messages[i].lifetime--;
    if(component.messages[i].lifetime <= 0) {
      delete_list.unshift(i);
    }
  }
  for(let i=0; i < delete_list.length; i++) {
    component.messages.splice(delete_list[i], 1);
  }
}, 100);

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
    mdbToastNotification,
    ftr: mdbFooter
  },
  methods: {
    connect:connect,
    onNotification: (message, title='', icon='info', color='elegant', lifetime=30) => {
      let newmessage = {id: component.message_id, title:title, message:message, icon:icon, color:color, lifetime:lifetime};
      component.message_id++;
      component.messages.push(newmessage);
    },
  },
  data() {
    return {
      show: true,
      message_id: 1,
      messages: [],
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
.placement {
  position: fixed;
  right: 10px;
  top: 10px;
  z-index:8000;
}
</style>

<style scoped>
main {
  background-color: #ededee;
}

.flexible-content {
  transition: padding-left 0.3s;
  padding-left: 270px;
  height: 100%;
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
