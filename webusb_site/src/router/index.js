import Vue from 'vue';
import Router from 'vue-router';
import Apps from '@/components/Apps'
import Programming from '@/components/Programming'
import Tables from '@/components/Tables'
import Maps from '@/components/Maps'
import BadGateway from '@/components/BadGateway'


Vue.use(Router);

export default new Router({
  mode: 'history',
  routes: [
    {
      path: '/apps',
      name: 'Apps',
      component: Apps,
      props: { page: 1 },
      alias: '/'
    },
    {
      path: '/programming',
      name: 'Programming & Files',
      props: { page: 2 },
      component: Programming
    },
    {
      path: '/tables',
      name: 'Tables',
      props: { page: 3 },
      component: Tables
    },
    {
      path: '/maps',
      name: 'Maps',
      props: { page: 4 },
      component: Maps
    },
    {
      path: '/404',
      name: 'BadGateway',
      props: { page: 5 },
      component: BadGateway
    },
    {
      path: '*',
      props: { page: 5 },
      redirect: '/404'
    }
  ]
})