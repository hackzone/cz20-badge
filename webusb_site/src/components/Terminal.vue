<template>
    <div class="xterm"></div>
</template>
<script>
    import { Terminal } from 'xterm';
    import { FitAddon } from 'xterm-addon-fit';
    import 'xterm/css/xterm.css';

    export default {
        data() {
            return {
                terminal: null,
                fitAddon: null,
                charCount: 0,
            }
        },
        mounted() {
            this.terminal = new Terminal({
                convertEol: true,
            });
            this.fitAddon = new FitAddon();
            this.terminal.loadAddon(this.fitAddon);
            this.terminal.open(this.$el);
            this.terminal.write(">>> ");
            this.fitAddon.fit();
            window.addEventListener('resize', this.handleResize);

            this.terminal.onData((data) => {
                this.$emit('data', data);
            });
        },
        beforeDestroy() {
            window.removeEventListener('resize', this.handleResize)
            this.terminal.dispose();
        },
        methods: {
            handleResize(event) {
                this.fitAddon.fit();
            },
            handleLog(str) {
                console.log('handleLog');
                this.terminal.write(str.replace('\n', '\r\n'));
            }
        }
    }
</script>
