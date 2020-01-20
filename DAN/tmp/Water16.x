xof 0303txt 0032

template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template FrameTransformMatrix {
 <F6F23F41-7686-11cf-8F52-0040333594A3>
 Matrix4x4 frameMatrix;
}

template Frame {
 <3D82AB46-62DA-11cf-AB39-0020AF71E433>
 [...]
}


template XSkinMeshHeader {
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}

template VertexDuplicationIndices {
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}

template SkinWeights {
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array FLOAT weights[nWeights];
 Matrix4x4 matrixOffset;
}


Material WaterTexcoord_Default {
 1.000000;1.000000;1.000000;1.000000;;
 51.200001;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;
 TextureFilename {
  "WaterBump.tga";
 }
}

Frame Frame_SCENE_ROOT {

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame Frame1_WaterTexcoord {

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh Mesh_WaterTexcoord {
   25;
   -0.500000;0.000000;-0.500000;,
   0.500000;0.000000;-0.500000;,
   0.500000;0.000000;0.500000;,
   -0.500000;0.000000;0.500000;,
   -0.500000;0.000000;0.000000;,
   0.000000;0.000000;0.500000;,
   0.500000;0.000000;0.000000;,
   0.000000;0.000000;-0.500000;,
   0.000000;0.000000;0.000000;,
   -0.250000;0.000000;-0.500000;,
   -0.500000;0.000000;-0.250000;,
   -0.250000;0.000000;0.000000;,
   0.000000;0.000000;-0.250000;,
   -0.250000;0.000000;-0.250000;,
   -0.500000;0.000000;0.250000;,
   -0.250000;0.000000;0.500000;,
   0.000000;0.000000;0.250000;,
   -0.250000;0.000000;0.250000;,
   0.250000;0.000000;0.500000;,
   0.500000;0.000000;0.250000;,
   0.250000;0.000000;0.000000;,
   0.250000;0.000000;0.250000;,
   0.500000;0.000000;-0.250000;,
   0.250000;0.000000;-0.500000;,
   0.250000;0.000000;-0.250000;;
   16;
   4;8,16,21,20;,
   4;9,0,10,13;,
   4;13,10,4,11;,
   4;12,13,11,8;,
   4;8,11,17,16;,
   4;11,4,14,17;,
   4;17,14,3,15;,
   4;16,17,15,5;,
   4;7,9,13,12;,
   4;16,5,18,21;,
   4;21,18,2,19;,
   4;20,21,19,6;,
   4;7,12,24,23;,
   4;12,8,20,24;,
   4;24,20,6,22;,
   4;23,24,22,1;;

   MeshNormals {
    25;
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;;
    16;
    4;8,16,21,20;,
    4;9,0,10,13;,
    4;13,10,4,11;,
    4;12,13,11,8;,
    4;8,11,17,16;,
    4;11,4,14,17;,
    4;17,14,3,15;,
    4;16,17,15,5;,
    4;7,9,13,12;,
    4;16,5,18,21;,
    4;21,18,2,19;,
    4;20,21,19,6;,
    4;7,12,24,23;,
    4;12,8,20,24;,
    4;24,20,6,22;,
    4;23,24,22,1;;
   }

   MeshTextureCoords {
    25;
    0.000000;1.000000;,
    1.000000;1.000000;,
    1.000000;0.000000;,
    0.000000;0.000000;,
    0.000000;0.500000;,
    0.500000;0.000000;,
    1.000000;0.500000;,
    0.500000;1.000000;,
    0.500000;0.500000;,
    0.250000;1.000000;,
    0.000000;0.750000;,
    0.250000;0.500000;,
    0.500000;0.750000;,
    0.250000;0.750000;,
    0.000000;0.250000;,
    0.250000;0.000000;,
    0.500000;0.250000;,
    0.250000;0.250000;,
    0.750000;0.000000;,
    1.000000;0.250000;,
    0.750000;0.500000;,
    0.750000;0.250000;,
    1.000000;0.750000;,
    0.750000;1.000000;,
    0.750000;0.750000;;
   }

   MeshMaterialList {
    1;
    16;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0;
    { WaterTexcoord_Default }
   }
  }
 }
}
