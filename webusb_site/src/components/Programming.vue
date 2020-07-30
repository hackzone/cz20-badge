<template>
  <section id='programming'>
    <mdb-card class='mb-4' ondrop='itemDrop'>
      <mdb-card-body>
        <section>
          <mdb-row>
            <mdb-col sm='6' md='4' lg='3'>
              <mdb-btn color='gray' size='lg' title='Make folder' v-on:click='mkdir_ui()' icon='folder-plus'></mdb-btn>
              <mdb-btn color='gray' size='lg' title='Make file' v-on:click='mkfile_ui()' icon='file'></mdb-btn>
              <mdb-btn color='gray' size='lg' title='Rename file' v-on:click='rename_ui()' icon='file-alt'></mdb-btn>
              <mdb-btn color='gray' size='lg' title='Delete file' v-on:click='trash_ui()' icon='trash'></mdb-btn>
              <mdb-btn color='gray' size='lg' title='Download folder' v-on:click='download_ui()' icon='download'></mdb-btn>
            </mdb-col>
            <mdb-col sm='2' md='2' lg='2'>
              <mdb-btn color='gray' size='lg' title='Run file' v-on:click='runfile_ui()' icon='play'></mdb-btn>
              <mdb-btn color='gray' size='lg' title='Save file' v-on:click='save_ui()' icon='save'></mdb-btn>
            </mdb-col>
            <mdb-col sm='4' md='6' lg='7' class="mt-2">
              <mdb-input label="Filename" v-model="editorfilename" size="md"/>
            </mdb-col>
          </mdb-row>
          <mdb-row class='mt-3'>
            <mdb-col sm='6' md='4' lg='3'>
              <v-jstree :data='files' draggable multiple allow-batch whole-row @item-click='itemClick' @item-toggle='itemToggle' @item-drop-before='itemDrop' @item-drop='otherDrop' @item-drag-start='itemDragStart'></v-jstree>
            </mdb-col>
            <mdb-col sm='6' md='8' lg='9'>
              <editor v-model='content_editor' lang='python' theme='monokai' height='500'></editor>
            </mdb-col>
          </mdb-row>
          </section>
      </mdb-card-body>
    </mdb-card>
      <mdb-card class='mb-4'>
      <mdb-card-body>
        <section>
              Python terminal
              <div class="md-form">
                  <Terminal ref="terminal" v-on:data="commandpython"></Terminal>
              </div>
        </section>
      </mdb-card-body>
    </mdb-card>
  </section>
</template>

<script>
window.itemDrop = function() {
  console.log('nope')
};

import {mdbToastNotification, mdbBtn, mdbCard, mdbCardBody, mdbCol, mdbRow, mdbInput} from 'mdbvue';
import VJstree from 'vue-jstree';
  import {connect, on_connect, runfile, readfile, savefile, fetch_dir, createfolder, savetextfile, movefile, delfile, deldir, createfile, registerstdout, writetostdin, downloaddir} from '../webusb';
import * as $ from 'jquery';
import * as ace from 'brace';
import 'brace/mode/python';
import 'brace/theme/monokai';
import * as ace_editor from 'vue2-ace-editor';
import { saveAs } from 'file-saver';
import * as JSZip from 'jszip';
import { default as Terminal } from './Terminal'

let component = undefined;
let selected_item = {model:{}};
let beforemoveloc = undefined;


const extension_whitelist = ["txt", "csv", "json", "py", "ini", "info", "md", "log", "conf", "cfg"];

function commandlog(str) {
  if(component && component.$refs && component.$refs.terminal) {
    component.$refs.terminal.handleLog(str);
  }
}

export default {
  name: 'Programming',
  components: {
    mdbBtn,
    mdbRow,
    mdbCol,
    mdbCard,
    mdbCardBody,
    mdbInput,
    VJstree,
    editor:ace_editor,
    Terminal,
  },
  beforeMount() {
    component = this;
    // Auto-fetch /flash
    registerstdout(commandlog);
    on_connect().then(() => this.itemClick({model: this.files[0]}));
  },
  methods: {
    parseDir: (data) => {
          let dir_structure = data.split('\n');
          console.log(dir_structure);
          let data_structure = [];
          let parent_path = dir_structure[0] === '/' ? '': dir_structure[0];
          for(let i = 1; i < dir_structure.length; i++) {
              let is_dir = dir_structure[i].charAt(0) === "d";
              let child = {};
              child["text"] = dir_structure[i].substr(1);
              child["full_path"] = parent_path + '/' + child["text"];
              if(is_dir) {
                  if(dir_structure[i] === "dflash") {
                      child["icon"] = "fas fa-microchip";
                  } else if(dir_structure[i] === "dsdcard") {
                      child["icon"] = "fas fa-sd-card";
                  } else {
                      child["icon"] = "far fa-folder";
                  }
                  child["is_dir"] = true;
                  child["dragDisabled"] = true;
              } else {
                  child["icon"] = "far fa-file";
              }
              child["opened"] = false;
              child["disabled"] = false;
              child["selected"] = false;
              if(is_dir) {
                  child["children"] = [{text:'Click parent to refresh', icon: 'none', isDummy: true}];
              }
              data_structure.push(child);
          }
          return data_structure;
    },
    updateNode:(node) => {
      let model = node.model;
      if(node.model.is_dir) {
        console.log("Updating: "+node.model.full_path);
        fetch_dir(model.full_path).then(data => {
          let children = component.parseDir(data);
          //Check for deleted items
          for(let i = 0; i < children.length; i++) {
            for (let origitem of model.children) {
              if(origitem.full_path === children[i].full_path) {
                children[i] = origitem;
              }
            }
          }
          console.log(children);
          model.children = children;
          model.opened = true;
        });
      } else {
        let parts = node.model.full_path.split(".");
        if(parts.length > 1 && extension_whitelist.indexOf(parts[parts.length-1].toLowerCase()) >= 0) {
          if(component.content_editor === component.content_original || window.confirm("Unsaved changes will be lost. Are you sure?")) {
            readfile(node.model.full_path).then((contents) => {
              component.content_editor = contents;
              component.content_original = contents;
              component.editorfilename = node.model.full_path;
              component.$emit('genNotification', 'Load success', 'Load success', 'check', 'green', 30);
              });
          }
        }        
      }
    },
    itemClick:(node) => {
      selected_item.model.selected = false;
      node.model.selected = true;
      selected_item = node;
      component.updateNode(node);      
    },
    itemToggle:(node) => {
      if (!node.data.is_dir) {
        return;
      }
      // Run only once
      if (!node.data.children.length || !node.data.children[0].isDummy) {
        return;
      }
      // And only for opened windows (this event gets fired twice)
      if (!node.data.opened) {
        return;
      }
      // And only for items we haven't seen before
      if (node.data.isToggled === true) {
        return;
      }

      node.data.isToggled = true;
      component.updateNode(node);
    },
    otherDrop: (node, item, d, e) => {

    },
    itemDrop: (node, item, draggedItem, e) => {
      if(draggedItem == undefined) {
        console.log("file upload");
        console.log(e);
        let entry = node.model.is_dir ? node : node.$parent;
        let path = entry.model.full_path;
        for(let index = 0; index < e.dataTransfer.files.length; index++) {
          let item = e.dataTransfer.files[index];
          let reader = new FileReader();
          reader.onload = function (event) {
            console.log(reader.result);
            savefile(path+"/"+item.name,reader.result);
            if(index == e.dataTransfer.files.length) {
              component.updateNode(entry);
            }
          };
          console.log(item);         
          reader.readAsArrayBuffer(item);
        }        
      } else {
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
      }
                  
    },
    itemDragStart: (node, item, e) => {
      let entry = node.model.is_dir ? node : node.$parent;
      console.log("Start:");
      console.log(entry);
      beforemoveloc = entry;
    },
    trash_ui: async () => {
      let file = selected_item.model.full_path;
      if(confirm("Delete: " + file + "?")) {
        if(selected_item.model.is_dir) {
          await deldir(file);
          if(file == component.editorfilename.substr(0, file.length)) {
            component.editorfilename = '/flash/cache/scratch.py';
            component.content_editor = '';
            component.content_original = '';
          } 
        } else {
          delfile(file);
          component.editorfilename = '/flash/cache/scratch.py';
          component.content_editor = '';
          component.content_original = '';
        }               
        component.itemClick(selected_item.$parent); // Refresh parent directory
      }
    },
    download_ui: async () => {
      if(selected_item.model.is_dir) {
        let file = selected_item.model.full_path;
        let zip = await downloaddir(file);
        zip.generateAsync({type:"blob"})
        .then(function (blob) {
            saveAs(blob, "download.zip");
            component.$emit('genNotification','Download succes', 'Download succes', 'check', 'green', 30);
        });
      } else {
        component.$emit('genNotification', 'Can only download folder','Download failed','times', 'red', 30);
      }
    },
    save_ui: async () => {
      let parts = component.editorfilename.split(".");
      if((parts.length > 1 && extension_whitelist.indexOf(parts[parts.length-1].toLowerCase()) >= 0) || window.confirm("File: "+component.editorfilename+" has not a textfile extension")) {
        await savetextfile(component.editorfilename, component.content_editor);
        component.content_original = component.content_editor;
        component.updateNode(selected_item.$parent);
        component.$emit('genNotification','Save succes', 'Save succes', 'check', 'green', 30);
      }
    },
    rename_ui: () => {
      if(selected_item.model.is_dir){ return; }

      let path = selected_item.model.full_path;
      let parent_path = selected_item.$parent.model.full_path;

      let filename = prompt("Please enter new filename", "");
      if (filename != null) {
        movefile(path, parent_path + '/' + filename);
        if(path === component.editorfilename) {
          component.editorfilename = parent_path + '/' + filename ;
        }
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
    runfile_ui: async () => {
      if(component.content_editor !== component.content_original) {
        await savetextfile(component.editorfilename, component.content_editor);
        component.content_original = component.content_editor;
      }
      let import_name = component.editorfilename.replace('/flash/', '/').replace('/__init__.py', '').replace('.py', '')
      if (import_name.length > 1 && import_name[0] === '/') {
        import_name = import_name.substr(1);
      }
      import_name = import_name.replace('/', '.')

      runfile(import_name);
    },
    info() {

    },
    commandpython(e) {
      writetostdin(e);
    },
    connect:connect,
  },
  data () {
    return {
      content_editor:'',
      content_original:'',
      editorfilename:'/flash/cache/scratch.py',
      show: true,
      files: [
        {
          text: 'flash',
          full_path: '/flash',
          icon: 'fas fa-microchip',
          is_dir: true,
          children: [{text:'Click parent to refresh', icon: 'none', isDummy: true}]
        },
        {
          text: 'sdcard',
          full_path: '/sdcard',
          icon: 'fas fa-sd-card',
          is_dir: true,
          children: [{text:'Click parent to refresh', icon: 'none', isDummy: true}]
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
  .btn.btn-lg {
    padding: .5rem .5rem !important;
  }
  .md-form {
   margin: 0 0 0 0;
  }
  input .input-lg {
    padding-top: 0 !important;
    padding-bottom: 0 !important;
  }
  textarea {
    width:100%;
    height:400px;
  }
</style>

<style>
  .md-form .form-control {
    margin-bottom: 0;
  }
  .tree-selected {
    background: #eee !important;
  }
</style>
