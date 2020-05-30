<template>
  <section id='programming'>
    <mdb-card class='mb-4' ondrop='itemDrop'>
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
                <mdb-btn color='gray' size='sm' title='Rename file' v-on:click='rename_ui()' icon='file-alt'></mdb-btn>
                <mdb-btn color='gray' size='sm' title='Upload file' v-on:click='upload_ui()' icon='upload'></mdb-btn>
                <mdb-btn color='gray' size='sm' title='Run file' v-on:click='startapp_ui()' icon='play'></mdb-btn>
              </mdb-col>
            </mdb-row>
            <mdb-row class='mt-3'>
              <mdb-col sm='6' md='4' lg='3'>
                <v-jstree :data='files' draggable multiple allow-batch whole-row @item-click='itemClick' @item-drop-before='itemDrop' @item-drag-start='itemDragStart'></v-jstree>
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
window.itemDrop = function() {
  console.log('nope')
}
  import {mdbBtn, mdbCard, mdbCardBody, mdbCol, mdbRow} from 'mdbvue';
import VJstree from 'vue-jstree';
import {trash_ui} from '../editor';
  import {readfile, fetch_dir, createfolder, savetextfile, movefile, delfile, createfile} from '../webusb';
import * as $ from 'jquery';
import * as ace from 'brace';
import 'brace/mode/python';
import 'brace/theme/monokai';
import * as ace_editor from 'vue2-ace-editor';
import {connect} from '../webusb';

let component = undefined;
let selected_item = {model:{}};
let beforemoveloc = undefined;

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
  beforeMount() {
    component = this;
  },
  methods: {
    updateNode:(node) => {
      let model = node.model
      if(node.model.is_dir) {
        console.log("Updating: "+node.model.full_path);
        fetch_dir(model.full_path, (children) => {
          //Check for deleted items
          console.log("new")
          console.log(children);
          console.log("old");
          console.log(model.children);
          console.log(model.full_path);
          for(let i = 0; i < children.length; i++) {
            for (let origitem of model.children) {
              if(origitem.full_path === children[i].full_path) {
                console.log("updating with");                
                children[i] = origitem;
                console.log(children[i].opened);
              }
            }
          }
          console.log(children);
          model.children = children;
          model.opened = true;
        });
      } else {
        readfile(node.model.full_path, (contents) => component.content = contents );
      }
    },
    itemClick:(node) => {
      selected_item.model.selected = false;
      node.model.selected = true;
      selected_item = node;
      component.updateNode(node);      
    },
    itemDrop: (node, item, draggedItem, e) => {
      console.log("drop");
      console.log(node);
      if(draggedItem.is_dir) return;
      let entry = node.model.is_dir ? node : node.$parent;
      let path = entry.model.full_path;
      let source = draggedItem.full_path;
      let destination = path + "/" + draggedItem.text;
      movefile(source, destination);            
      component.updateNode(beforemoveloc);
      component.updateNode(entry);
                  
    },
    itemDragStart: (node, item, e) => {
      let entry = node.model.is_dir ? node : node.$parent;
      console.log("Start:");
      console.log(entry);
      beforemoveloc = entry;
    },
    trash_ui: () => {
      let file = selected_item.model.full_path;
      if(confirm("Delete: " + file + "?")) {
        delfile(file);
        component.itemClick(selected_item.$parent); // Refresh parent directory
      }
    },
    save_ui: () => savetextfile(selected_item.model.full_path, component.content),
    rename_ui: () => {
      if(selected_item.model.is_dir){ return; }

      let path = selected_item.model.full_path;
      let parent_path = selected_item.$parent.model.full_path;

      let filename = prompt("Please enter new filename", "");
      if (filename != null) {
        movefile(path, parent_path + '/' + filename);
        component.itemClick(selected_item.$parent); // Refresh parent directory
      }
    },
    mkdir_ui: () => {
      let dirname = prompt("Please enter new directory name", "");
      let entry = selected_item.model.is_dir ? selected_item : selected_item.$parent;
      let path = entry.model.full_path;
      if(dirname) {
        createfolder(path + '/' + dirname);
        component.itemClick(entry); // Refresh parent directory
      }
    },
    mkfile_ui: () => {
      let filename = prompt("Please enter new filename", "");
      let entry = selected_item.model.is_dir ? selected_item : selected_item.$parent;
      let path = entry.model.full_path;
      if(filename) {
        createfile(path + '/' + filename);
        component.itemClick(entry); // Refresh parent directory
      }
    },
    connect:connect,
  },
  data () {
    return {
      content:'',
      files: [
        {
          text: 'flash',
          full_path: '/flash',
          icon: 'fas fa-microchip',
          is_dir: true,
          children: [{text:'Click parent to refresh', icon: 'none'}]
        },
        {
          text: 'sdcard',
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

<style>
  .tree-selected {
    background: #eee !important;
  }
</style>
