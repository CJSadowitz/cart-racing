# Cart-Racing
There is no make file for the project (I didn't know they existed at the time)
Currently has a functioning scene renderer;
  shaders can be duplicated from the examples in the files
  designed to read obj files that are built with quads not triangles. (Triangles will cause crashes, 'poles' are supported)
There is preliminary code for a naive BVH. It only partially functions and the visualization is poor and full of bugs.
The GUI code is set up to take 3D obj files on the XZ plane and transforms it to the correct orientation. Clickable buttons will be automatically attached to the objects, however the functionality must be manually programmed.
