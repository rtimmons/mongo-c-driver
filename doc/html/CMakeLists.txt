# The html distribution list was set by sphinx_build_html ()
#extra_dist_generated (${doc_DIST_htmls})

file (GLOB_RECURSE _images_pngs
   RELATIVE ${PROJECT_SOURCE_DIR}/doc/static
   ${PROJECT_SOURCE_DIR}/doc/static/*.png
)
foreach (png IN LISTS _images_pngs)
   extra_dist_generated (_images/${png})
endforeach ()

# Additional generated static files
extra_dist_generated (
   .nojekyll
   objects.inv
   _static/ajax-loader.gif
   _static/basic.css
   _static/comment-bright.png
   _static/comment-close.png
   _static/comment.png
   _static/doctools.js
   _static/down-pressed.png
   _static/down.png
   _static/file.png
   _static/jquery.js
   _static/minus.png
   _static/mongoc.css
   _static/plus.png
   _static/pygments.css
   _static/searchtools.js
   _static/underscore.js
   _static/up-pressed.png
   _static/up.png
   _static/websupport.js
   genindex.html
   search.html
   searchindex.js
)

set_dist_list (doc_html_DIST
   CMakeLists.txt
)
