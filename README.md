3D_model_viewer for generation synthetic dataset
=================================================

1. Path to .obj files you can select in main.cpp
2. In function "display" there is cycle, wich save every frame from render into selected location (function "SaveScene")
3. In skybox.cpp you can change background through change path to cuba-map texture files. Examples of it cotains in ./          skybox/images/
4. Examples of .obj models you can find in ./Models
5. Control instruction will be displayed in terminal window.

Sorry for bad-code. I don't try to make it perfect)


Examples of render images
=========================
Render image:

<img src="https://github.com/NickLyrick/SynthDataGen/blob/master/For_segmentation_example/images/havoc_0.png" alt="Screenshot1" border="1" height="640" width="640">

Labeled image:

<img src="https://github.com/NickLyrick/SynthDataGen/blob/master/For_segmentation_example/labels/havoc_0.png" alt="Screenshot2" border="1" height="640" width="640">

This kind of images used by me for learning CNN for semantic segmetation. 
