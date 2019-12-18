xof 0302txt 0064
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

Header{
1;
0;
1;
}

Mesh {
 182;
 0.00000;5.18263;0.00000;,
 0.08935;3.78267;-0.05159;,
 0.00000;3.78267;-0.10318;,
 0.00000;5.18263;0.00000;,
 0.08935;3.78267;0.05159;,
 0.00000;5.18263;0.00000;,
 0.00000;3.78267;0.10318;,
 0.00000;5.18263;0.00000;,
 -0.08935;3.78267;0.05159;,
 0.00000;5.18263;0.00000;,
 -0.08935;3.78267;-0.05159;,
 0.00000;5.18263;0.00000;,
 0.00000;3.78267;-0.10318;,
 -0.13317;3.42013;-0.07688;,
 -0.13317;3.42013;0.07688;,
 -0.15699;3.07296;-0.09064;,
 -0.18142;2.38118;-0.10475;,
 -0.18142;2.38118;0.10475;,
 -0.15699;3.07296;0.09064;,
 -0.20449;1.98550;-0.11806;,
 -0.20449;1.98550;0.11806;,
 -0.22148;1.44650;-0.12787;,
 -0.22148;1.44650;0.12787;,
 -0.23050;0.77322;-0.13308;,
 -0.25981;-1.00000;-0.15000;,
 -0.25981;-1.00000;0.15000;,
 -0.23050;0.77322;0.13308;,
 0.00000;0.77322;-0.26616;,
 0.00000;-1.00000;-0.30000;,
 0.00000;1.44650;-0.25574;,
 0.00000;1.98550;-0.23613;,
 0.00000;2.89313;-0.18128;,
 0.00000;2.41384;-0.20949;,
 0.00000;3.42013;-0.15377;,
 0.00000;3.07296;0.18128;,
 0.00000;3.42013;0.15377;,
 0.00000;2.52260;0.20949;,
 0.00000;1.98550;0.23613;,
 0.00000;1.44650;0.25574;,
 0.00000;0.77322;0.26616;,
 0.00000;-1.00000;0.30000;,
 0.00000;-1.00000;-0.30000;,
 0.00000;0.77322;-0.26616;,
 0.23050;0.77322;-0.13308;,
 0.25981;-1.00000;-0.15000;,
 0.00000;1.44650;-0.25574;,
 0.22148;1.44650;-0.12787;,
 0.00000;1.98550;-0.23613;,
 0.20449;1.98550;-0.11806;,
 0.00000;2.41384;-0.20949;,
 0.18142;2.41969;-0.10475;,
 0.00000;2.89313;-0.18128;,
 0.00000;3.42013;-0.15377;,
 0.13317;3.42013;-0.07688;,
 0.10710;2.79651;-0.09320;,
 0.13317;3.42013;0.07688;,
 0.15699;2.80627;0.09064;,
 0.18142;2.52260;0.10475;,
 0.22148;1.44650;0.12787;,
 0.20449;1.98550;0.11806;,
 0.23050;0.77322;0.13308;,
 0.25981;-1.00000;0.15000;,
 0.20547;2.92252;0.59496;,
 0.38690;2.91067;0.49021;,
 0.34755;3.18377;0.84104;,
 0.52897;3.18377;0.73630;,
 0.54620;3.32328;1.03105;,
 0.68688;3.32328;0.94983;,
 0.34755;2.90917;0.86768;,
 0.54620;3.11035;1.05170;,
 0.20547;2.63607;0.62160;,
 0.40997;2.63607;0.50353;,
 0.55204;2.90917;0.74961;,
 0.70477;3.11035;0.96015;,
 0.57758;2.20044;-0.09109;,
 0.59069;1.92559;-0.09866;,
 0.86711;2.32911;-0.09064;,
 0.88015;2.05563;-0.09817;,
 1.09725;2.73305;-0.06821;,
 1.10706;2.52724;-0.07388;,
 0.86711;2.32911;0.09064;,
 1.09725;2.73305;0.06821;,
 0.57758;2.20044;0.09109;,
 0.59069;1.92559;0.09866;,
 0.88015;2.05563;0.09817;,
 1.10706;2.52724;0.07388;,
 -1.42946;2.47928;0.05450;,
 -1.04807;2.16806;0.07843;,
 -1.04807;2.16806;-0.07843;,
 -1.42946;2.47928;-0.05450;,
 -0.65510;1.97264;0.10217;,
 -0.65510;1.97264;-0.10217;,
 -0.66231;1.59385;0.10632;,
 -1.05360;1.87726;0.08163;,
 -1.43330;2.27723;0.05672;,
 -1.05360;1.87726;-0.08163;,
 -1.43330;2.27723;-0.05672;,
 -0.66231;1.59385;-0.10632;,
 -0.11603;3.93070;0.29339;,
 -0.11603;3.61855;0.33695;,
 -0.21846;4.09613;0.45636;,
 -0.21846;3.85165;0.49048;,
 -0.35450;4.16861;0.67961;,
 -0.35450;3.98278;0.70554;,
 -0.27872;4.09613;0.42157;,
 -0.40030;4.16861;0.65317;,
 -0.19296;3.93070;0.24897;,
 -0.23069;3.61855;0.27075;,
 -0.30826;3.85165;0.43863;,
 -0.42275;3.98278;0.66613;,
 -1.16722;3.62334;-0.02684;,
 -0.90665;3.53616;-0.04184;,
 -0.91962;3.34724;-0.04932;,
 -1.17554;3.50214;-0.03164;,
 -0.45631;3.55949;-0.05587;,
 -0.47363;3.30719;-0.06587;,
 -0.45631;3.55949;0.05587;,
 -0.90665;3.53616;0.04184;,
 -1.16722;3.62334;0.02684;,
 -0.91962;3.34724;0.04932;,
 -1.17554;3.50214;0.03164;,
 -0.47363;3.30719;0.06587;,
 -0.50077;2.70947;-0.71621;,
 -0.38555;2.63156;-0.48612;,
 -0.23166;2.63156;-0.57497;,
 -0.37153;2.70947;-0.79083;,
 -0.27596;2.43364;-0.27061;,
 -0.09453;2.43364;-0.37535;,
 -0.29903;2.15904;-0.28393;,
 -0.40512;2.39864;-0.49742;,
 -0.51720;2.51386;-0.72570;,
 -0.23166;2.39864;-0.59757;,
 -0.37153;2.51386;-0.80980;,
 -0.09453;2.15904;-0.40199;,
 0.22759;3.04605;-0.53400;,
 0.22759;2.81018;-0.55746;,
 0.31839;3.18609;-0.68827;,
 0.31839;2.96940;-0.70982;,
 0.47253;3.38209;-0.92634;,
 0.47253;3.20896;-0.94356;,
 0.43831;3.18609;-0.61904;,
 0.56835;3.38209;-0.87103;,
 0.35813;3.04605;-0.45864;,
 0.37844;2.81018;-0.47037;,
 0.45697;2.96940;-0.62981;,
 0.58326;3.20896;-0.87963;,
 0.36413;4.07092;-0.57991;,
 0.29689;3.98965;-0.51574;,
 0.33692;3.98965;-0.49263;,
 0.39692;4.07092;-0.56098;,
 0.17390;3.87467;-0.34155;,
 0.22325;3.87467;-0.31306;,
 0.17390;3.67441;-0.36950;,
 0.29689;3.82724;-0.53841;,
 0.36413;3.93788;-0.59848;,
 0.35655;3.82724;-0.50397;,
 0.41300;3.93788;-0.57027;,
 0.24746;3.67441;-0.32703;,
 -1.78702;3.11078;0.00000;,
 -1.78702;3.11078;0.00000;,
 -1.78702;3.11078;0.00000;,
 1.48182;3.09367;0.00000;,
 1.48182;3.09367;0.00000;,
 1.48182;3.09367;0.00000;,
 0.77283;3.57542;1.26264;,
 0.77283;3.57542;1.26264;,
 0.77283;3.57542;1.26264;,
 -0.64496;3.05579;-1.11710;,
 -0.64496;3.05579;-1.11710;,
 -0.64496;3.05579;-1.11710;,
 -1.36205;3.74670;-0.00000;,
 -1.36205;3.74670;-0.00000;,
 -1.36205;3.74670;-0.00000;,
 -0.50046;4.24374;0.87954;,
 -0.50046;4.24374;0.87954;,
 -0.50046;4.24374;0.87954;,
 0.51700;4.14205;-0.77458;,
 0.51700;4.14205;-0.77458;,
 0.51700;4.14205;-0.77458;,
 0.70702;3.56261;-1.22459;,
 0.70702;3.56261;-1.22459;,
 0.70702;3.56261;-1.22459;;
 
 168;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 4;10,13,14,8;,
 4;15,16,17,18;,
 4;16,19,20,17;,
 4;19,21,22,20;,
 4;23,24,25,26;,
 4;24,23,27,28;,
 4;23,21,29,27;,
 4;21,19,30,29;,
 4;16,15,31,32;,
 4;15,13,33,31;,
 4;13,10,12,33;,
 4;14,18,34,35;,
 4;18,17,36,34;,
 4;17,20,37,36;,
 4;20,22,38,37;,
 4;22,26,39,38;,
 4;26,25,40,39;,
 4;41,42,43,44;,
 4;42,45,46,43;,
 4;45,47,48,46;,
 4;47,49,50,48;,
 4;51,52,53,54;,
 4;6,35,55,4;,
 4;35,34,56,55;,
 4;34,36,57,56;,
 4;37,38,58,59;,
 4;38,39,60,58;,
 4;39,40,61,60;,
 4;44,43,60,61;,
 4;43,46,58,60;,
 4;48,50,57,59;,
 4;50,54,56,57;,
 4;54,53,55,56;,
 4;53,1,4,55;,
 4;36,62,63,57;,
 4;62,64,65,63;,
 4;64,66,67,65;,
 4;66,64,68,69;,
 4;64,62,70,68;,
 4;62,36,37,70;,
 4;57,63,71,59;,
 4;63,65,72,71;,
 4;65,67,73,72;,
 4;69,68,72,73;,
 4;68,70,71,72;,
 4;70,37,59,71;,
 4;48,74,75,46;,
 4;74,76,77,75;,
 4;76,78,79,77;,
 4;78,76,80,81;,
 4;76,74,82,80;,
 4;74,48,59,82;,
 4;46,75,83,58;,
 4;75,77,84,83;,
 4;77,79,85,84;,
 4;81,80,84,85;,
 4;80,82,83,84;,
 4;82,59,58,83;,
 4;86,87,88,89;,
 4;87,90,91,88;,
 4;90,22,21,91;,
 4;22,90,92,26;,
 4;90,87,93,92;,
 4;87,86,94,93;,
 4;89,88,95,96;,
 4;88,91,97,95;,
 4;91,21,23,97;,
 4;26,92,97,23;,
 4;92,93,95,97;,
 4;93,94,96,95;,
 4;6,98,99,35;,
 4;98,100,101,99;,
 4;100,102,103,101;,
 4;102,100,104,105;,
 4;100,98,106,104;,
 4;98,6,8,106;,
 4;35,99,107,14;,
 4;99,101,108,107;,
 4;101,103,109,108;,
 4;105,104,108,109;,
 4;104,106,107,108;,
 4;106,8,14,107;,
 4;110,111,112,113;,
 4;111,114,115,112;,
 4;114,13,15,115;,
 4;13,114,116,14;,
 4;114,111,117,116;,
 4;111,110,118,117;,
 4;113,112,119,120;,
 4;112,115,121,119;,
 4;115,15,18,121;,
 4;14,116,121,18;,
 4;116,117,119,121;,
 4;117,118,120,119;,
 4;122,123,124,125;,
 4;123,126,127,124;,
 4;126,16,32,127;,
 4;16,126,128,19;,
 4;126,123,129,128;,
 4;123,122,130,129;,
 4;125,124,131,132;,
 4;124,127,133,131;,
 4;127,32,30,133;,
 4;19,128,133,30;,
 4;128,129,131,133;,
 4;129,130,132,131;,
 4;51,134,135,49;,
 4;134,136,137,135;,
 4;136,138,139,137;,
 4;138,136,140,141;,
 4;136,134,142,140;,
 4;134,51,54,142;,
 4;49,135,143,50;,
 4;135,137,144,143;,
 4;137,139,145,144;,
 4;141,140,144,145;,
 4;140,142,143,144;,
 4;142,54,50,143;,
 4;146,147,148,149;,
 4;147,150,151,148;,
 4;150,2,1,151;,
 4;2,150,152,52;,
 4;150,147,153,152;,
 4;147,146,154,153;,
 4;149,148,155,156;,
 4;148,151,157,155;,
 4;151,1,53,157;,
 4;52,152,157,53;,
 4;152,153,155,157;,
 4;153,154,156,155;,
 3;158,89,96;,
 3;159,96,94;,
 3;86,89,158;,
 3;160,94,86;,
 3;79,78,161;,
 3;85,79,162;,
 3;161,78,81;,
 3;81,85,163;,
 3;66,69,164;,
 3;164,69,73;,
 3;67,66,165;,
 3;166,73,67;,
 3;130,122,167;,
 3;167,122,125;,
 3;168,125,132;,
 3;132,130,169;,
 3;170,110,113;,
 3;171,113,120;,
 3;118,110,170;,
 3;172,120,118;,
 3;173,102,105;,
 3;174,105,109;,
 3;103,102,173;,
 3;175,109,103;,
 3;176,146,149;,
 3;154,146,176;,
 3;177,149,156;,
 3;156,154,178;,
 3;139,138,179;,
 3;179,138,141;,
 3;145,139,180;,
 3;181,141,145;;
 
 MeshMaterialList {
  1;
  168;
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
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "af18.jpg";
   }
  }
 }
 MeshNormals {
  306;
  0.498985;0.063696;-0.864267;,
  0.498985;0.063696;-0.864267;,
  0.997969;0.063696;0.000000;,
  0.995683;0.092814;0.000000;,
  0.980776;0.088165;0.174084;,
  0.498985;0.063696;0.864267;,
  0.668293;0.085756;0.738938;,
  0.497842;0.092814;0.862287;,
  -0.498985;0.063696;0.864267;,
  -0.498985;0.063696;0.864267;,
  -0.997969;0.063696;0.000000;,
  -0.995683;0.092814;0.000000;,
  -0.975211;0.086316;-0.203746;,
  -0.498985;0.063696;-0.864267;,
  -0.626531;0.089938;-0.774190;,
  -0.497842;0.092814;-0.862287;,
  -0.992776;0.119980;0.000000;,
  -0.973965;0.103639;-0.201623;,
  -0.906799;-0.421564;-0.000000;,
  -0.980415;0.045982;0.191499;,
  -0.868159;0.054086;0.493331;,
  -0.883373;-0.292563;0.366142;,
  -0.992153;0.044722;0.116755;,
  -0.946254;0.052453;0.319142;,
  -0.999504;0.031501;0.000000;,
  -0.973489;-0.228734;-0.000000;,
  -0.999863;0.016527;0.000000;,
  -0.499932;0.016527;-0.865907;,
  -0.443687;0.014492;-0.896065;,
  -0.499944;0.014967;-0.865928;,
  -0.309590;0.017479;-0.950709;,
  -0.499875;0.022371;-0.865809;,
  -0.461020;-0.387093;-0.798511;,
  -0.460783;-0.388224;-0.798099;,
  -0.488068;0.042636;-0.871764;,
  -0.383087;0.051925;-0.922252;,
  -0.641415;0.045851;-0.765823;,
  -0.627981;0.043497;-0.777012;,
  -0.328157;0.072400;-0.941845;,
  -0.575422;0.087360;-0.813178;,
  -0.311512;-0.206882;0.927448;,
  -0.328886;0.059911;0.942467;,
  -0.504600;0.053976;0.861664;,
  -0.466213;-0.361360;0.807505;,
  -0.701426;0.039868;0.711627;,
  -0.585575;0.040077;0.809627;,
  -0.499752;0.031501;0.865596;,
  -0.309590;0.017479;0.950709;,
  -0.499875;0.022371;0.865809;,
  -0.443687;0.014492;0.896065;,
  -0.499944;0.014967;0.865928;,
  -0.499932;0.016527;0.865907;,
  0.499932;0.016527;-0.865907;,
  0.499944;0.014967;-0.865928;,
  0.499944;0.014967;-0.865928;,
  0.499875;0.022371;-0.865809;,
  0.426392;0.023213;-0.904241;,
  0.597542;0.042525;-0.800710;,
  0.556969;0.044630;-0.829333;,
  0.650923;-0.223221;-0.725584;,
  0.860860;-0.156574;-0.484153;,
  0.567686;0.019584;-0.823012;,
  0.503329;-0.371958;-0.779941;,
  0.871081;-0.173430;-0.459499;,
  0.937437;0.061645;-0.342653;,
  0.611212;0.083679;0.787031;,
  0.511224;0.086214;0.855112;,
  0.524835;0.056275;0.849342;,
  0.525670;0.062459;0.848393;,
  0.525911;0.064279;0.848107;,
  0.480551;-0.276194;0.832339;,
  0.499875;0.022371;0.865809;,
  0.426392;0.023213;0.904241;,
  0.318449;-0.156815;0.934879;,
  0.499944;0.014967;0.865928;,
  0.499944;0.014967;0.865928;,
  0.499932;0.016527;0.865907;,
  0.999863;0.016527;0.000000;,
  0.999888;0.014967;0.000000;,
  0.950701;-0.310108;0.000000;,
  0.954749;0.076143;-0.287501;,
  0.986308;0.045629;-0.158479;,
  0.985290;0.068142;-0.156715;,
  0.995902;0.059812;-0.067842;,
  -0.316634;0.742596;-0.590165;,
  -0.320832;0.735342;-0.596941;,
  -0.320570;0.735800;-0.596518;,
  -0.316634;0.742596;-0.590165;,
  -0.285753;0.806682;-0.517310;,
  -0.280336;0.816020;-0.505494;,
  -0.269020;0.842919;-0.465957;,
  -0.273241;0.837470;-0.473268;,
  -0.735798;0.065388;0.674037;,
  -0.775224;0.060699;0.628763;,
  -0.840112;0.051730;0.539941;,
  -0.721971;0.066809;0.688691;,
  -0.888914;0.040317;0.456296;,
  -0.894605;0.034632;0.445514;,
  0.848716;0.042293;-0.527155;,
  0.857459;0.038484;-0.513111;,
  0.808399;0.039429;-0.587313;,
  0.827829;0.042416;-0.559375;,
  0.904586;0.055497;-0.422664;,
  0.837004;0.043864;-0.545436;,
  0.362427;-0.688901;0.627744;,
  0.328787;-0.753390;0.569476;,
  0.326654;-0.757093;0.565780;,
  0.368116;-0.676729;0.637596;,
  0.382813;-0.643286;0.663052;,
  0.383231;-0.642291;0.663774;,
  0.012813;0.026295;-0.999572;,
  0.032819;0.025657;-0.999132;,
  0.027571;0.028826;-0.999204;,
  0.000877;0.027564;-0.999620;,
  0.124012;0.033219;-0.991725;,
  0.064346;0.030532;-0.997460;,
  -0.790615;0.612314;0.000000;,
  -0.667087;0.744980;0.000000;,
  -0.454202;0.890899;0.000000;,
  -0.499184;0.866496;0.000000;,
  0.622670;-0.782485;0.000000;,
  0.693713;-0.720252;0.000000;,
  0.871248;-0.490843;0.000000;,
  0.124012;0.033219;0.991725;,
  0.027571;0.028826;0.999204;,
  0.000877;0.027564;0.999620;,
  0.064346;0.030532;0.997461;,
  0.012813;0.026295;0.999572;,
  0.032819;0.025657;0.999132;,
  0.759520;0.650484;0.000000;,
  0.539051;0.842273;0.000000;,
  0.627035;0.778991;0.000000;,
  0.771692;0.635997;0.000000;,
  -0.052023;0.011497;0.998580;,
  -0.047833;0.010593;0.998799;,
  -0.053381;0.011978;0.998502;,
  -0.053956;0.011989;0.998471;,
  -0.108411;0.012975;0.994021;,
  -0.064275;0.012178;0.997858;,
  -0.108411;0.012975;-0.994021;,
  -0.053381;0.011978;-0.998502;,
  -0.053956;0.011989;-0.998471;,
  -0.064275;0.012178;-0.997858;,
  -0.052023;0.011497;-0.998580;,
  -0.047833;0.010593;-0.998799;,
  -0.759548;-0.650451;-0.000000;,
  -0.656587;-0.754250;-0.000000;,
  -0.832827;-0.553533;-0.000000;,
  0.874076;0.067141;0.481126;,
  0.874123;0.067129;0.481043;,
  0.866299;0.069039;0.494732;,
  0.869933;0.068161;0.488437;,
  0.838841;0.075238;0.539153;,
  0.849539;0.072909;0.522463;,
  0.139560;0.960256;-0.241726;,
  0.237092;0.880426;-0.410655;,
  0.238177;0.879254;-0.412533;,
  0.139530;0.960274;-0.241673;,
  0.301581;0.797620;-0.522352;,
  0.301325;0.798006;-0.521909;,
  0.276755;0.832843;-0.479353;,
  -0.365571;-0.682225;0.633187;,
  -0.365322;-0.682760;0.632755;,
  -0.319948;-0.768462;0.554166;,
  -0.320770;-0.767090;0.555590;,
  -0.306400;-0.790239;0.530699;,
  -0.306041;-0.790795;0.530078;,
  -0.889535;0.076070;-0.450490;,
  -0.870577;0.071242;-0.486847;,
  -0.868020;0.070610;-0.491482;,
  -0.882402;0.074223;-0.464604;,
  -0.863741;0.069561;-0.499111;,
  -0.862167;0.069177;-0.501878;,
  -0.082405;0.045101;-0.995578;,
  -0.038453;0.042195;-0.998369;,
  -0.037335;0.042120;-0.998415;,
  -0.059920;0.043624;-0.997250;,
  -0.039332;0.042254;-0.998332;,
  -0.040933;0.042361;-0.998264;,
  0.395992;0.918254;0.000000;,
  0.183343;0.983049;0.000000;,
  0.129980;0.991517;0.000000;,
  0.423170;0.906050;0.000000;,
  0.423170;0.906050;0.000000;,
  -0.660797;-0.750564;-0.000000;,
  -0.304875;-0.952392;-0.000000;,
  -0.660797;-0.750564;-0.000000;,
  -0.363544;-0.931577;-0.000000;,
  -0.039332;0.042254;0.998332;,
  -0.040933;0.042361;0.998264;,
  -0.038453;0.042195;0.998369;,
  -0.037335;0.042120;0.998415;,
  -0.082405;0.045101;0.995578;,
  -0.059920;0.043624;0.997250;,
  0.230746;0.887145;0.399663;,
  0.238307;0.879112;0.412760;,
  0.238611;0.878783;0.413286;,
  0.231062;0.886816;0.400210;,
  0.175362;0.907305;0.382158;,
  0.174927;0.907612;0.381628;,
  0.026890;0.980201;0.196169;,
  0.026890;0.980201;0.196169;,
  -0.865898;0.045817;0.498118;,
  -0.861327;0.042686;0.506254;,
  -0.880741;0.051156;0.470827;,
  -0.876381;0.050395;0.478975;,
  -0.908942;0.056326;0.413100;,
  -0.896409;0.053971;0.439931;,
  0.815482;0.055885;-0.576078;,
  0.846394;0.051422;-0.530068;,
  0.846321;0.051433;-0.530184;,
  0.833002;0.053422;-0.550685;,
  0.858936;0.046056;-0.510008;,
  0.867878;0.042104;-0.494990;,
  -0.345521;-0.722815;-0.598459;,
  -0.345491;-0.722873;-0.598407;,
  -0.287960;-0.817507;-0.498760;,
  -0.288192;-0.817179;-0.499163;,
  -0.307853;-0.787978;-0.533215;,
  -0.308180;-0.787466;-0.533782;,
  -0.868667;0.045054;-0.493344;,
  -0.859300;0.043171;-0.509647;,
  -0.864233;0.049787;-0.500622;,
  -0.872576;0.048341;-0.486081;,
  -0.822131;0.056340;-0.566503;,
  -0.842010;0.053387;-0.536814;,
  -0.253242;0.862249;0.438629;,
  -0.296614;0.805034;0.513751;,
  -0.296752;0.804831;0.513990;,
  -0.253285;0.862198;0.438704;,
  -0.155175;0.844336;0.512852;,
  -0.158815;0.843265;0.513499;,
  -0.003277;0.877504;0.479559;,
  0.343445;-0.735040;-0.584603;,
  0.343507;-0.734980;-0.584643;,
  0.329806;-0.751608;-0.571239;,
  0.329854;-0.751523;-0.571323;,
  0.337249;-0.738276;-0.584133;,
  0.337238;-0.738297;-0.584113;,
  0.903034;0.056603;0.425824;,
  0.872097;0.050911;0.486677;,
  0.873515;0.051160;0.484101;,
  0.885627;0.053338;0.461324;,
  0.852634;0.065137;0.518433;,
  0.828060;0.079271;0.555007;,
  0.806858;0.090595;0.583757;,
  0.806858;0.090595;0.583757;,
  -0.255342;0.859771;0.442260;,
  -0.284678;0.822094;0.493072;,
  -0.292324;0.811289;0.506316;,
  -0.239805;0.877484;0.415350;,
  -0.192183;0.923181;0.332870;,
  -0.194574;0.921175;0.337011;,
  -0.149606;0.954187;0.259125;,
  -0.149606;0.954187;0.259125;,
  -0.830124;0.077063;-0.552228;,
  -0.825472;0.078012;-0.559026;,
  -0.778758;0.086702;-0.621304;,
  -0.813744;0.080330;-0.575645;,
  -0.779625;0.086553;-0.620237;,
  -0.760007;0.089824;-0.643678;,
  0.816706;0.058645;0.574067;,
  0.764190;0.047472;0.643242;,
  0.800145;0.055024;0.597277;,
  0.769546;0.048573;0.636742;,
  0.847975;0.065783;0.525939;,
  0.861259;0.068956;0.503467;,
  0.877075;0.072868;0.474794;,
  0.320166;-0.768099;-0.554543;,
  0.319100;-0.769871;-0.552696;,
  0.344467;-0.724824;-0.596634;,
  0.351282;-0.711622;-0.608437;,
  0.368979;-0.674848;-0.639090;,
  0.364866;-0.683733;-0.631967;,
  -0.127638;0.013316;-0.991732;,
  -0.920546;-0.390634;-0.000000;,
  0.870194;0.492709;0.000000;,
  -0.127637;0.013316;0.991732;,
  0.143476;0.034074;-0.989067;,
  0.833987;-0.551784;0.000000;,
  -0.684025;0.729458;0.000000;,
  0.143476;0.034074;0.989067;,
  -0.747274;0.064162;0.661411;,
  0.421814;-0.536926;0.730605;,
  -0.313132;0.779611;-0.542361;,
  0.947764;0.064474;-0.312389;,
  -0.922064;0.058911;0.382527;,
  0.318419;0.770998;0.551517;,
  0.794869;0.058588;-0.603946;,
  -0.401281;-0.596571;-0.695037;,
  -0.107247;0.046709;-0.993134;,
  -0.795153;-0.606409;-0.000000;,
  0.534959;0.844878;0.000000;,
  -0.107247;0.046709;0.993134;,
  0.145985;0.956427;-0.252854;,
  -0.901597;0.079289;-0.425248;,
  0.819075;0.079289;0.568181;,
  -0.379635;-0.650778;0.657545;,
  -0.139385;0.960359;0.241419;,
  -0.798294;0.083239;-0.596488;,
  0.859711;0.068581;0.506156;,
  0.332780;-0.746344;-0.576392;,
  -0.801832;0.059136;-0.594616;,
  -0.216624;0.901275;0.375204;,
  0.350088;-0.713970;-0.606371;,
  0.918567;0.059686;0.390734;;
  168;
  3;0,1,1;,
  3;2,3,4;,
  3;5,6,7;,
  3;8,9,9;,
  3;10,11,12;,
  3;13,14,15;,
  4;11,16,17,12;,
  4;18,19,20,21;,
  4;19,22,23,20;,
  4;22,24,24,23;,
  4;25,26,26,25;,
  4;27,28,29,27;,
  4;28,30,31,29;,
  4;30,32,33,31;,
  4;34,35,36,37;,
  4;35,38,39,36;,
  4;38,15,14,39;,
  4;40,41,42,43;,
  4;21,20,44,42;,
  4;20,23,45,44;,
  4;46,47,48,45;,
  4;47,49,50,48;,
  4;49,51,51,50;,
  4;52,53,54,52;,
  4;53,55,56,54;,
  4;55,57,58,56;,
  4;57,59,60,58;,
  4;61,62,63,64;,
  4;6,65,66,7;,
  4;65,67,68,66;,
  4;67,69,69,68;,
  4;70,71,72,73;,
  4;71,74,75,72;,
  4;74,76,76,75;,
  4;77,78,78,77;,
  4;78,79,79,78;,
  4;58,60,80,81;,
  4;60,64,82,80;,
  4;64,63,83,82;,
  4;63,4,3,83;,
  4;84,85,86,87;,
  4;85,88,89,86;,
  4;88,90,91,89;,
  4;92,93,94,95;,
  4;93,96,97,94;,
  4;96,44,45,97;,
  4;80,98,99,81;,
  4;98,100,101,99;,
  4;100,102,103,101;,
  4;104,105,106,107;,
  4;105,108,109,106;,
  4;108,70,73,109;,
  4;58,110,111,56;,
  4;110,112,113,111;,
  4;112,114,115,113;,
  4;116,117,117,116;,
  4;117,118,118,117;,
  4;118,119,119,118;,
  4;79,120,120,79;,
  4;120,121,121,120;,
  4;121,122,122,121;,
  4;123,124,125,126;,
  4;124,127,128,125;,
  4;127,73,72,128;,
  4;129,130,130,129;,
  4;130,131,131,130;,
  4;131,132,132,131;,
  4;47,133,134,49;,
  4;133,135,136,134;,
  4;135,137,138,136;,
  4;139,140,141,142;,
  4;140,143,144,141;,
  4;143,30,28,144;,
  4;25,145,145,25;,
  4;145,146,146,145;,
  4;146,147,147,146;,
  4;6,148,149,65;,
  4;148,150,151,149;,
  4;150,152,153,151;,
  4;154,155,156,157;,
  4;155,158,159,156;,
  4;158,160,160,159;,
  4;43,161,162,40;,
  4;161,163,164,162;,
  4;163,165,166,164;,
  4;167,168,169,170;,
  4;168,171,172,169;,
  4;171,12,17,172;,
  4;173,174,175,176;,
  4;174,177,178,175;,
  4;177,38,35,178;,
  4;179,180,180,179;,
  4;180,181,181,180;,
  4;181,182,183,181;,
  4;184,185,185,186;,
  4;185,187,187,185;,
  4;187,18,21,187;,
  4;40,188,189,41;,
  4;188,190,191,189;,
  4;190,192,193,191;,
  4;194,195,196,197;,
  4;195,198,199,196;,
  4;198,200,201,199;,
  4;19,202,203,22;,
  4;202,204,205,203;,
  4;204,206,207,205;,
  4;208,209,210,211;,
  4;209,212,213,210;,
  4;212,59,57,213;,
  4;32,214,215,33;,
  4;214,216,217,215;,
  4;216,218,219,217;,
  4;36,220,221,37;,
  4;220,222,223,221;,
  4;222,224,225,223;,
  4;226,227,228,229;,
  4;227,230,231,228;,
  4;230,232,232,231;,
  4;59,233,234,60;,
  4;233,235,236,234;,
  4;235,237,238,236;,
  4;239,240,241,242;,
  4;240,243,244,241;,
  4;243,245,246,244;,
  4;247,248,249,250;,
  4;248,251,252,249;,
  4;251,253,254,252;,
  4;14,255,256,39;,
  4;255,257,258,256;,
  4;257,259,260,258;,
  4;261,262,263,264;,
  4;262,265,266,263;,
  4;265,4,267,266;,
  4;62,268,269,63;,
  4;268,270,271,269;,
  4;270,272,273,271;,
  3;274,139,142;,
  3;275,147,147;,
  3;129,129,276;,
  3;277,138,137;,
  3;115,114,278;,
  3;122,122,279;,
  3;280,116,116;,
  3;123,126,281;,
  3;92,95,282;,
  3;283,104,107;,
  3;91,90,284;,
  3;285,103,102;,
  3;207,206,286;,
  3;287,194,197;,
  3;288,208,211;,
  3;219,218,289;,
  3;290,173,176;,
  3;291,184,186;,
  3;183,182,292;,
  3;293,193,192;,
  3;294,154,157;,
  3;295,167,170;,
  3;153,152,296;,
  3;297,166,165;,
  3;298,247,250;,
  3;260,259,299;,
  3;300,261,264;,
  3;273,272,301;,
  3;225,224,302;,
  3;303,226,229;,
  3;238,237,304;,
  3;305,239,242;;
 }
 MeshTextureCoords {
  182;
  0.083333;0.000000;,
  0.166667;0.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.333333;0.000000;,
  0.416667;0.000000;,
  0.500000;0.000000;,
  0.583333;0.000000;,
  0.666667;0.000000;,
  0.750000;0.000000;,
  0.833333;0.000000;,
  0.916667;0.000000;,
  1.000000;0.000000;,
  0.833333;0.142857;,
  0.666667;0.142857;,
  0.833333;0.285714;,
  0.833333;0.428571;,
  0.666667;0.428571;,
  0.666667;0.285714;,
  0.833333;0.571429;,
  0.666667;0.571429;,
  0.833333;0.714286;,
  0.666667;0.714286;,
  0.833333;0.857143;,
  0.833333;1.000000;,
  0.666667;1.000000;,
  0.666667;0.857143;,
  1.000000;0.857143;,
  1.000000;1.000000;,
  1.000000;0.714286;,
  1.000000;0.571429;,
  1.000000;0.285714;,
  1.000000;0.428571;,
  1.000000;0.142857;,
  0.500000;0.285714;,
  0.500000;0.142857;,
  0.500000;0.428571;,
  0.500000;0.571429;,
  0.500000;0.714286;,
  0.500000;0.857143;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.857143;,
  0.166667;0.857143;,
  0.166667;1.000000;,
  0.000000;0.714286;,
  0.166667;0.714286;,
  0.000000;0.571429;,
  0.166667;0.571429;,
  0.000000;0.428571;,
  0.166667;0.428571;,
  0.000000;0.285714;,
  0.000000;0.142857;,
  0.166667;0.142857;,
  0.166667;0.285714;,
  0.333333;0.142857;,
  0.333333;0.285714;,
  0.333333;0.428571;,
  0.333333;0.714286;,
  0.333333;0.571429;,
  0.333333;0.857143;,
  0.333333;1.000000;,
  0.500000;0.428571;,
  0.333333;0.428571;,
  0.500000;0.428571;,
  0.333333;0.428571;,
  0.500000;0.428571;,
  0.333333;0.428571;,
  0.500000;0.571429;,
  0.500000;0.571429;,
  0.500000;0.571429;,
  0.333333;0.571429;,
  0.333333;0.571429;,
  0.333333;0.571429;,
  0.166667;0.571429;,
  0.166667;0.714286;,
  0.166667;0.571429;,
  0.166667;0.714286;,
  0.166667;0.571429;,
  0.166667;0.714286;,
  0.333333;0.571429;,
  0.333333;0.571429;,
  0.333333;0.571429;,
  0.333333;0.714286;,
  0.333333;0.714286;,
  0.333333;0.714286;,
  0.666667;0.714286;,
  0.666667;0.714286;,
  0.833333;0.714286;,
  0.833333;0.714286;,
  0.666667;0.714286;,
  0.833333;0.714286;,
  0.666667;0.857143;,
  0.666667;0.857143;,
  0.666667;0.857143;,
  0.833333;0.857143;,
  0.833333;0.857143;,
  0.833333;0.857143;,
  0.500000;0.000000;,
  0.500000;0.142857;,
  0.500000;0.000000;,
  0.500000;0.142857;,
  0.500000;0.000000;,
  0.500000;0.142857;,
  0.666667;0.000000;,
  0.666667;0.000000;,
  0.666667;0.000000;,
  0.666667;0.142857;,
  0.666667;0.142857;,
  0.666667;0.142857;,
  0.833333;0.142857;,
  0.833333;0.142857;,
  0.833333;0.285714;,
  0.833333;0.285714;,
  0.833333;0.142857;,
  0.833333;0.285714;,
  0.666667;0.142857;,
  0.666667;0.142857;,
  0.666667;0.142857;,
  0.666667;0.285714;,
  0.666667;0.285714;,
  0.666667;0.285714;,
  0.833333;0.428571;,
  0.833333;0.428571;,
  1.000000;0.428571;,
  1.000000;0.428571;,
  0.833333;0.428571;,
  1.000000;0.428571;,
  0.833333;0.571429;,
  0.833333;0.571429;,
  0.833333;0.571429;,
  1.000000;0.571429;,
  1.000000;0.571429;,
  1.000000;0.571429;,
  0.000000;0.285714;,
  0.000000;0.428571;,
  0.000000;0.285714;,
  0.000000;0.428571;,
  0.000000;0.285714;,
  0.000000;0.428571;,
  0.166667;0.285714;,
  0.166667;0.285714;,
  0.166667;0.285714;,
  0.166667;0.428571;,
  0.166667;0.428571;,
  0.166667;0.428571;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.166667;0.000000;,
  0.166667;0.000000;,
  0.000000;0.000000;,
  0.166667;0.000000;,
  0.000000;0.142857;,
  0.000000;0.142857;,
  0.000000;0.142857;,
  0.166667;0.142857;,
  0.166667;0.142857;,
  0.166667;0.142857;,
  0.833333;0.714286;,
  0.833333;0.857143;,
  0.666667;0.857143;,
  0.166667;0.571429;,
  0.166667;0.714286;,
  0.333333;0.714286;,
  0.500000;0.571429;,
  0.500000;0.428571;,
  0.333333;0.571429;,
  0.833333;0.428571;,
  1.000000;0.428571;,
  0.833333;0.571429;,
  0.833333;0.142857;,
  0.833333;0.285714;,
  0.666667;0.285714;,
  0.500000;0.000000;,
  0.666667;0.000000;,
  0.666667;0.142857;,
  0.000000;0.000000;,
  0.166667;0.000000;,
  0.000000;0.142857;,
  0.000000;0.285714;,
  0.000000;0.428571;,
  0.166667;0.285714;;
 }
}