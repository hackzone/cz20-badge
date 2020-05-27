<template>
  <section id="programming">
    <mdb-card class="mb-4">
      <mdb-card-body>
        <button onclick="connect()">Connect</button>
        <h1>WebUSB Development</h1>
        <div id="wrapper">
          <input id="file-input" type="file" name="name" style="display: none;" />

          <div id="icons">
            <button class="btn" title="Delete file" v-on:click="trash_ui()"><i class="fa fa-trash"></i></button>
            <button class="btn" title="Save file" onclick="save_ui()"><i class="fa fa-save"></i></button>
            <button class="btn" title="Make folder" onclick="mkdir_ui()"><i class="fa fa-folder-plus"></i></button>
            <button class="btn" title="Make file" onclick="mkfile_ui()"><i class="fa fa-file"></i></button>
            <button class="btn" title="Sync" onclick="sync_ui()"><i class="fa fa-sync"></i></button>
            <button class="btn" title="Rename file" onclick="rename_ui()"><i class="far fa-file-alt"></i></button>
            <button class="btn" title="Upload file" onclick="upload_ui()"><i class="fas fa-upload"></i></button>
            <button class="btn" title="Run file" onclick="startapp_ui()"><i class="fas fa-play"></i></button>
            <button class="btn" title="Copy file" onclick="copy_ui()"><i class="fas fa-copy"></i></button>
            <button class="btn" title="Cut file" onclick="cut_ui()"><i class="fas fa-cut"></i></button>
            <button class="btn" title="Paste file" id="paste_btn" onclick="paste_ui()" disabled><i class="fas fa-paste"></i></button>
          </div>
          <div id="filebrowser" class="filebrowser"></div>

          <editor v-model="content" lang="python" theme="monokai" height="500" @init="initEditor"></editor>
        </div>
      </mdb-card-body>
    </mdb-card>
  </section>
</template>

<script>
import { mdbCard, mdbCardBody } from 'mdbvue';

import {editor, trash_ui, readSingleFile} from '../editor';
import {treechanged, fetch_dir} from '../webusb';
import * as $ from 'jquery';
import * as ace from 'brace';
window.ace = ace;
import 'brace/mode/python';
import 'brace/theme/monokai';
import * as ace_editor from 'vue2-ace-editor';

export default {
  name: 'Programming',
  components: {
    mdbCard,
    mdbCardBody,
    editor:ace_editor
  },
  methods: {
    initEditor:function (editor) {

      document.getElementById('file-input')
          .addEventListener('change', readSingleFile, false);

      //Init jstree
      // $('#filebrowser')
      //     .on('changed.jstree', treechanged)
      //     .jstree({
      //         "core" : {
      //             "multiple" : false,
      //             "data" : fetch_dir,
      //             "check_callback" : true
      //         }
      //     });
    },
    trash_ui: trash_ui
  },
  data () {
    return {
      content:""
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
</style>
