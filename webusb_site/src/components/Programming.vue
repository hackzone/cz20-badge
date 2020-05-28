<template>
  <section id='programming'>
    <mdb-card class='mb-4'>
      <mdb-card-body>
        <section>
        <mdb-row>
          <mdb-col>
            <mdb-row>
              <mdb-col>
                <mdb-btn color='gray' size='sm' title='Delete file' v-on:click='trash_ui()' icon='trash'></mdb-btn>
                <mdb-btn color='gray' size='sm' title='Save file' v-on:click='save_ui()' icon='save'></mdb-btn>
                <mdb-btn color='gray' size='sm' title='Make folder' v-on:click='mkdir_ui()' icon='folder-plus'></mdb-btn>
                <mdb-btn color='gray' size='sm' title='Make file' v-on:click='mkfile_ui()' icon='file'></mdb-btn>
                <mdb-btn color='gray' size='sm' title='Sync' v-on:click='sync_ui()' icon='sync'></mdb-btn>
                <mdb-btn color='gray' size='sm' title='Rename file' v-on:click='rename_ui()' icon='file-alt'></mdb-btn>
                <mdb-btn color='gray' size='sm' title='Upload file' v-on:click='upload_ui()' icon='upload'></mdb-btn>
                <mdb-btn color='gray' size='sm' title='Run file' v-on:click='startapp_ui()' icon='play'></mdb-btn>
                <mdb-btn color='gray' size='sm' title='Copy file' v-on:click='copy_ui()' icon='copy'></mdb-btn>
                <mdb-btn color='gray' size='sm' title='Cut file' v-on:click='cut_ui()' icon='cut'></mdb-btn>
                <mdb-btn color='gray' size='sm' title='Paste file' icon='paste' id='paste_btn' v-on:click='paste_ui()'></mdb-btn>
              </mdb-col>
            </mdb-row>
            <mdb-row class='mt-3'>
              <mdb-col sm='6' md='4' lg='3'>
                <v-jstree :data='files' multiple allow-batch whole-row @item-click='itemClick'></v-jstree>
              </mdb-col>
              <mdb-col sm='6' md='8' lg='9'>
                <editor v-model='content' lang='python' theme='monokai' height='500' @init='initEditor'></editor>
              </mdb-col>
            </mdb-row>
          </mdb-col>
        </mdb-row>
        </section>
      </mdb-card-body>
    </mdb-card>
  </section>
</template>

<script>
  import {mdbBtn, mdbCard, mdbCardBody, mdbCol, mdbRow} from 'mdbvue';
import VJstree from 'vue-jstree';
import {editor, trash_ui, readSingleFile} from '../editor';
import {treechanged, fetch_dir} from '../webusb';
import * as $ from 'jquery';
import * as ace from 'brace';
import 'brace/mode/python';
import 'brace/theme/monokai';
import * as ace_editor from 'vue2-ace-editor';
import {connect} from '../webusb';
window.fetch_dir=fetch_dir;

function updateNode(node) {
  fetch_dir(node.model.full_path, (children) => {
    node.model.children = children;
    node.model.opened = true;
  });
}

export default {
  name: 'Programming',
  components: {
    mdbBtn,
    mdbRow,
    mdbCol,
    mdbCard,
    mdbCardBody,
    VJstree,
    editor:ace_editor
  },
  methods: {
    initEditor:function (editor) {
    },
    itemClick:updateNode,
    // itemToggle (node) {
    //   if (node.model.children !== undefined && node.model.children.length === 1 &&
    //           node.model.children[0]['icon'] === 'none') {
    //     updateNode(node);
    //   }
    // },
    trash_ui: trash_ui,
    connect:connect,
  },
  data () {
    return {
      content:'test',
      files: [
        {
          text: 'flash',
          selected: true,
          full_path: '/flash',
          icon: 'fas fa-microchip',
          is_dir: true,
          children: [{text:'Click parent to refresh', icon: 'none'}]
        },
        {
          text: 'sdcard',
          selected: true,
          full_path: '/sdcard',
          icon: 'fas fa-sd-card',
          is_dir: true,
          children: [{text:'Click parent to refresh', icon: 'none'}]
        }
      ]
    }
  }
}
</script>

<style scoped>
  .profile-card-footer {
    background-color: #F7F7F7 !important;
    padding: 1.25rem;
  }
  .card.card-cascade .view {
    box-shadow: 0 3px 10px 0 rgba(0, 0, 0, 0.15), 0 3px 12px 0 rgba(0, 0, 0, 0.15);
  }
  .btn.btn-sm {
    padding: .5rem .5rem !important;
  }
</style>
