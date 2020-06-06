import Vue from 'vue';
import Router from 'vue-router';
import Apps from '@/components/Apps'
import Programming from '@/components/Programming'
import Settings from '@/components/Settings'
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
      path: '/settings',
      name: 'Settings',
      props: { page: 3 },
      component: Settings
    },
    {
      path: '/404',
      name: 'BadGateway',
      props: { page: 4 },
      component: BadGateway
    },
    {
      path: '*',
      props: { page: 4 },
      redirect: '/404'
    }
  ]
})