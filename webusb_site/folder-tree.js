var folderTree = (function () {

    // Generate a folder tree from a nested text list.
    function folderTree(text_list, customRender) {

        // HTML rendering functions
        var Render = {
            icon : function(node) {
                return node.children
                    ? '<i class="icon-folder-close"></i>'
                    : '<i class="icon-file-alt"></i>';
            }
            ,
            name : function(node) {
                return '<span class="name">' + trim(node.name) + '</span>';
            }
            ,
            classes : function(nodes) {
                var classes = "depth-" + nodes[0].depth;
                if (nodes[0].depth === 0) {
                    classes += " folder-tree";
                }
                return classes;
            }
        }

        if(customRender) {
            for (var name in customRender) { Render[name] = customRender[name] }
        }

        return nestedArrayToHTML(
            flatArrayToNestedArray(
                textListToFlatArray(text_list)
            )
        );


        function nestedArrayToHTML(nodes) {
            var html = '<ul class="'+ Render.classes(nodes) +'">',
                i = 0,
                node;

            while(node = nodes[i++]) {
                html += "<li>";
                html += Render.icon(node);
                html += Render.name(node);
                if(node.children && node.children.length > 0) {
                    html += nestedArrayToHTML(node.children);
                }
                html += '</li>';
            }

            html += '</ul>';

            return html;
        }

        function flatArrayToNestedArray(nodes) {
            var output = [];
            var i = 0, node;
            while(node = nodes[i++]) {
                var reference = output
                    ,depth = node.depth;

                while(depth--) {
                    if(!reference[reference.length-1].children) {
                        reference[reference.length-1].children = [];
                    }
                    reference = reference[reference.length-1].children;
                }

                reference.push(node);
            }

            return output;
        }

        function textListToFlatArray(sub_context) {
            var lines = sub_context.match(/[^\r\n]+/g),
                length = lines.length,
                indent_position = lines[0].toString().search(/\S/),
                results = [],
                i = 0,
                line;

            for (var i = 0; i < length; ++i ) {
                var line = lines[i],
                    new_line = line.substring(indent_position),
                    name = trim(line),
                    indent = new_line.search(/\S/),
                    depth = (indent / 2),
                    data = {
                        indent: indent,
                        name: name,
                        depth: depth
                    };

                if(name === '') continue;
                var prevIndent = results[i-1] ? results[i-1].indent : false;
                if(!isValidNesting(prevIndent, indent)) {
                    throw new Error(
                        "Illegal nesting: "
                        + indent +" spaces at line #" + (i+1) + ' "' + name + '"'
                    );
                }

                // assume no period is a folder-name.
                if (line.indexOf('.') === -1) {
                    data.children = [] ;
                }

                results.push(data);
            }

            return results;
        }

        function isValidNesting(previous, current) {
            return (
                current%2 === 0
                && ( !previous || (current - 2) <= previous ) 
            );
        }

        function trim(string){
            return string.replace(/(^\s+|\s+$)/g, '');
        }
    }

    folderTree.version = '0.0.1';

    return folderTree;
}());
