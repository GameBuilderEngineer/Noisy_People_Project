xof 0303txt 0032

Frame Root {
  FrameTransformMatrix {
     1.000000, 0.000000, 0.000000, 0.000000,
     0.000000,-0.000000, 1.000000, 0.000000,
     0.000000, 1.000000, 0.000000, 0.000000,
     0.000000, 0.000000, 0.000000, 1.000000;;
  }
  Frame Plane {
    FrameTransformMatrix {
      50000.007812, 0.000000, 0.000000, 0.000000,
       0.000000,50000.007812, 0.000000, 0.000000,
       0.000000, 0.000000,50000.007812, 0.000000,
       0.000000, 0.000000, 0.000000, 1.000000;;
    }
    Mesh { // Plane mesh
      16;
       0.000000; 0.000000; 0.000000;,
       1.000000; 0.000000; 0.000000;,
       1.000000; 1.000000; 0.000000;,
       0.000000; 1.000000; 0.000000;,
      -1.000000; 0.000000; 0.000000;,
       0.000000; 0.000000; 0.000000;,
       0.000000; 1.000000; 0.000000;,
      -1.000000; 1.000000; 0.000000;,
      -1.000000;-1.000000; 0.000000;,
       0.000000;-1.000000; 0.000000;,
       0.000000; 0.000000; 0.000000;,
      -1.000000; 0.000000; 0.000000;,
       0.000000;-1.000000; 0.000000;,
       1.000000;-1.000000; 0.000000;,
       1.000000; 0.000000; 0.000000;,
       0.000000; 0.000000; 0.000000;;
      4;
      4;3,2,1,0;,
      4;7,6,5,4;,
      4;11,10,9,8;,
      4;15,14,13,12;;
      MeshNormals { // Plane normals
        4;
         0.000000; 0.000000; 1.000000;,
         0.000000; 0.000000; 1.000000;,
         0.000000; 0.000000; 1.000000;,
         0.000000; 0.000000; 1.000000;;
        4;
        4;0,0,0,0;,
        4;1,1,1,1;,
        4;2,2,2,2;,
        4;3,3,3,3;;
      } // End of Plane normals
      MeshTextureCoords { // Plane UV coordinates
        16;
         0.500000; 0.500000;,
         6.748753; 0.500000;,
         6.748753;-5.748753;,
         0.500000;-5.748753;,
        -5.748749; 0.500000;,
         0.500000; 0.500000;,
         0.500000;-5.748753;,
        -5.748753;-5.748753;,
        -5.748749; 6.748753;,
         0.500000; 6.748753;,
         0.500000; 0.500000;,
        -5.748749; 0.500000;,
         0.500000; 6.748753;,
         6.748753; 6.748753;,
         6.748753; 0.500000;,
         0.500000; 0.500000;;
      } // End of Plane UV coordinates
      MeshMaterialList { // Plane material list
        1;
        4;
        0,
        0,
        0,
        0;
        Material Material {
           0.640000; 0.640000; 0.640000; 1.000000;;
           96.078431;
           0.500000; 0.500000; 0.500000;;
           0.000000; 0.000000; 0.000000;;
          TextureFilename {"ocean.png";}
        }
      } // End of Plane material list
    } // End of Plane mesh
  } // End of Plane
} // End of Root
