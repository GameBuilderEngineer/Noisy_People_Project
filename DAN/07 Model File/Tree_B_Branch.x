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
 196;
 0.43301;-0.49921;-0.25000;,
 0.39387;0.61395;-0.18750;,
 0.39387;0.61395;0.18750;,
 0.43301;-0.49921;0.25000;,
 0.00000;-0.49921;-0.50000;,
 0.06911;0.61395;-0.37500;,
 0.06911;0.61395;0.37500;,
 0.00000;-0.49921;0.50000;,
 0.06911;0.61395;-0.37500;,
 0.00000;-0.49921;-0.50000;,
 -0.43301;-0.49921;-0.25000;,
 -0.25565;0.61395;-0.18750;,
 -0.25565;0.61395;0.18750;,
 -0.43301;-0.49921;0.25000;,
 0.35026;3.44083;0.07796;,
 0.40616;3.17822;0.09109;,
 0.40597;3.09847;-0.15213;,
 0.35000;3.55738;-0.02340;,
 0.53880;2.61526;0.12500;,
 0.53880;2.61526;-0.12500;,
 0.40616;3.17822;0.09109;,
 0.24839;3.17714;0.18229;,
 0.42161;2.50000;0.25000;,
 0.35026;3.44083;0.07796;,
 0.21523;3.43933;0.15607;,
 0.40597;3.09847;-0.15213;,
 0.28459;2.47829;-0.23538;,
 0.24839;3.09954;-0.24300;,
 0.24839;3.17714;0.18229;,
 0.09062;3.05730;0.09109;,
 0.20510;2.50000;0.12500;,
 0.21523;3.43933;0.15607;,
 0.08021;3.44083;0.05782;,
 0.21523;3.55887;-0.10106;,
 0.24839;3.09954;-0.24300;,
 0.09080;3.05945;-0.09098;,
 0.08046;3.44381;-0.05767;,
 0.28459;2.47829;-0.23538;,
 0.04955;2.45417;-0.10770;,
 0.09062;3.05730;0.09109;,
 0.09080;3.05945;-0.09098;,
 0.59038;2.14879;0.14256;,
 0.59038;2.14879;-0.14256;,
 0.34346;2.14879;0.28512;,
 0.20663;1.99287;-0.35378;,
 0.09653;2.14879;0.14256;,
 0.20663;1.99287;-0.35378;,
 -0.04108;1.95916;-0.21669;,
 -0.03116;4.01267;0.08346;,
 0.13133;3.83075;0.12127;,
 0.05537;3.80318;0.08168;,
 -0.08878;3.99154;0.05341;,
 0.35000;3.55738;-0.02340;,
 0.21523;3.55887;-0.10106;,
 0.13133;3.83075;0.12127;,
 0.13114;3.85278;0.20868;,
 -0.03116;4.01267;0.08346;,
 -0.03148;4.02970;0.14977;,
 -0.08878;3.99154;0.05341;,
 0.05537;3.80318;0.08168;,
 -0.02090;3.79759;0.12935;,
 -0.14679;3.98741;0.08961;,
 0.13114;3.85278;0.20868;,
 0.05479;3.84721;0.25654;,
 -0.14679;3.98741;0.08961;,
 -0.02090;3.79759;0.12935;,
 -0.02129;3.81958;0.21679;,
 -0.14720;4.00442;0.15595;,
 0.08046;3.44381;-0.05767;,
 0.08021;3.44083;0.05782;,
 0.05479;3.84721;0.25654;,
 -0.02129;3.81958;0.21679;,
 -0.08952;4.02558;0.18606;,
 -0.14720;4.00442;0.15595;,
 -0.03148;4.02970;0.14977;,
 0.01217;4.27545;0.34733;,
 0.10248;4.19022;0.43459;,
 -0.04716;4.59097;0.42269;,
 -0.00156;4.55510;0.48320;,
 -0.09197;4.98020;0.50993;,
 -0.04011;4.98487;0.55940;,
 -0.06997;4.58742;0.42531;,
 -0.10857;4.97174;0.51694;,
 -0.02851;4.27154;0.35729;,
 -0.08952;4.02558;0.18606;,
 0.04895;4.18501;0.44777;,
 -0.03158;4.55039;0.48669;,
 -0.06194;4.97372;0.56867;,
 1.63604;3.56024;0.00917;,
 1.37884;3.25927;0.03274;,
 1.51480;3.15893;0.04729;,
 1.74898;3.45380;0.03438;,
 0.87201;3.07266;0.11257;,
 0.98235;2.87492;0.11768;,
 0.86427;3.07327;-0.04682;,
 1.38360;3.25841;-0.08665;,
 1.64456;3.55484;-0.06916;,
 1.52023;3.15794;-0.08887;,
 1.75869;3.44765;-0.05496;,
 0.97352;2.87561;-0.06410;,
 -0.40176;3.82238;0.03386;,
 -0.48170;3.69592;0.05135;,
 -0.72726;4.15235;0.02198;,
 -0.78354;4.06987;0.03361;,
 -0.95847;4.28428;0.01153;,
 -0.99035;4.23757;0.01809;,
 -0.72651;4.15320;-0.02392;,
 -0.95804;4.28476;-0.01446;,
 -0.40068;3.82369;-0.03489;,
 -0.48092;3.69687;-0.05228;,
 -0.78300;4.07048;-0.03557;,
 -0.99004;4.23792;-0.02109;,
 -0.29452;3.01273;-0.34771;,
 -0.38191;2.72748;-0.40048;,
 -0.60343;3.66240;-0.82314;,
 -0.67981;3.41308;-0.86926;,
 -0.98486;3.84467;-1.27646;,
 -1.04469;3.64941;-1.31258;,
 -0.46925;3.68722;-0.94034;,
 -0.87978;3.86411;-1.36825;,
 -0.14100;3.04113;-0.48181;,
 -0.20683;2.75986;-0.55342;,
 -0.52679;3.44140;-1.00292;,
 -0.92484;3.67158;-1.41727;,
 0.67330;4.43636;-1.08301;,
 0.42965;4.08519;-0.53865;,
 0.53463;4.08315;-0.48037;,
 0.73307;4.43572;-1.05131;,
 0.28519;3.80989;-0.31294;,
 0.42268;3.80840;-0.24001;,
 0.31741;3.54603;-0.34049;,
 0.48583;3.90314;-0.61747;,
 0.68731;4.32164;-1.09499;,
 0.60850;3.90122;-0.54913;,
 0.75721;4.32118;-1.05789;,
 0.47818;3.54496;-0.25515;,
 -1.35086;4.65543;-1.92828;,
 -1.35086;4.65543;-1.92828;,
 -1.35086;4.65543;-1.92828;,
 -0.22965;5.32095;0.51627;,
 -0.22965;5.32095;0.51627;,
 -0.22965;5.32095;0.51627;,
 -1.19350;4.41065;-0.00200;,
 -1.19350;4.41065;-0.00200;,
 -1.19350;4.41065;-0.00200;,
 0.84682;4.91480;-1.32826;,
 0.84682;4.91480;-1.32826;,
 0.84682;4.91480;-1.32826;,
 2.21905;3.86496;0.01173;,
 2.21905;3.86496;0.01173;,
 2.21905;3.86496;0.01173;,
 -0.25103;2.88797;1.42611;,
 -0.05209;2.90922;0.90276;,
 0.00276;3.15569;0.87811;,
 -0.20893;3.07719;1.40719;,
 0.13899;2.64510;0.58759;,
 0.19928;2.91607;0.56049;,
 -0.05152;2.64510;0.47760;,
 -0.22538;2.90922;0.80271;,
 -0.38407;2.88797;1.34930;,
 -0.14919;3.15569;0.79039;,
 -0.32558;3.07719;1.33984;,
 0.03224;2.91607;0.46405;,
 -0.47408;3.55467;1.97789;,
 -0.47408;3.55467;1.97789;,
 -0.47408;3.55467;1.97789;,
 -0.06781;4.28188;0.10352;,
 -0.11185;4.25555;0.08096;,
 -0.06812;4.29560;0.15511;,
 -0.06781;4.28188;0.10352;,
 -0.11251;4.28325;0.18520;,
 -0.06812;4.29560;0.15511;,
 -0.15746;4.25665;0.16323;,
 -0.11251;4.28325;0.18520;,
 -0.15711;4.24266;0.11058;,
 -0.15746;4.25665;0.16323;,
 -0.11185;4.25555;0.08096;,
 -0.15711;4.24266;0.11058;,
 0.00095;4.69756;0.08821;,
 0.18499;5.23179;0.19321;,
 0.02585;4.70601;0.10123;,
 0.02585;4.70601;0.10123;,
 0.18499;5.23179;0.19321;,
 0.02654;4.70735;0.13085;,
 0.02654;4.70735;0.13085;,
 0.18499;5.23179;0.19321;,
 0.00171;4.70004;0.14778;,
 0.00171;4.70004;0.14778;,
 0.18499;5.23179;0.19321;,
 -0.02374;4.69140;0.13439;,
 -0.02374;4.69140;0.13439;,
 0.18499;5.23179;0.19321;,
 -0.02382;4.69025;0.10445;,
 -0.02382;4.69025;0.10445;,
 0.18499;5.23179;0.19321;,
 0.00095;4.69756;0.08821;;
 
 150;
 4;0,1,2,3;,
 4;1,0,4,5;,
 4;3,2,6,7;,
 4;8,9,10,11;,
 4;7,6,12,13;,
 4;11,10,13,12;,
 4;14,15,16,17;,
 4;15,18,19,16;,
 4;18,20,21,22;,
 4;20,23,24,21;,
 4;25,19,26,27;,
 4;22,28,29,30;,
 4;28,31,32,29;,
 4;33,34,35,36;,
 4;34,37,38,35;,
 4;30,39,40,38;,
 4;41,2,1,42;,
 4;2,41,43,6;,
 4;41,18,22,43;,
 4;19,42,44,26;,
 4;42,1,5,44;,
 4;6,43,45,12;,
 4;46,8,11,47;,
 4;12,45,47,11;,
 4;45,30,38,47;,
 4;48,49,50,51;,
 4;49,52,53,50;,
 4;17,54,55,14;,
 4;54,56,57,55;,
 4;58,59,60,61;,
 4;59,33,36,60;,
 4;23,62,63,24;,
 4;64,65,66,67;,
 4;65,68,69,66;,
 4;31,70,71,32;,
 4;70,72,73,71;,
 4;74,75,76,62;,
 4;75,77,78,76;,
 4;77,79,80,78;,
 4;79,77,81,82;,
 4;77,75,83,81;,
 4;75,74,84,83;,
 4;62,76,85,63;,
 4;76,78,86,85;,
 4;78,80,87,86;,
 4;82,81,86,87;,
 4;81,83,85,86;,
 4;83,84,63,85;,
 4;88,89,90,91;,
 4;89,92,93,90;,
 4;92,18,41,93;,
 4;18,92,94,19;,
 4;92,89,95,94;,
 4;89,88,96,95;,
 4;91,90,97,98;,
 4;90,93,99,97;,
 4;93,41,42,99;,
 4;19,94,99,42;,
 4;94,95,97,99;,
 4;95,96,98,97;,
 4;69,100,101,39;,
 4;100,102,103,101;,
 4;102,104,105,103;,
 4;104,102,106,107;,
 4;102,100,108,106;,
 4;100,69,68,108;,
 4;39,101,109,40;,
 4;101,103,110,109;,
 4;103,105,111,110;,
 4;107,106,110,111;,
 4;106,108,109,110;,
 4;108,68,40,109;,
 4;38,112,113,47;,
 4;112,114,115,113;,
 4;114,116,117,115;,
 4;116,114,118,119;,
 4;114,112,120,118;,
 4;112,38,37,120;,
 4;47,113,121,46;,
 4;113,115,122,121;,
 4;115,117,123,122;,
 4;119,118,122,123;,
 4;118,120,121,122;,
 4;120,37,46,121;,
 4;124,125,126,127;,
 4;125,128,129,126;,
 4;128,53,52,129;,
 4;53,128,130,27;,
 4;128,125,131,130;,
 4;125,124,132,131;,
 4;127,126,133,134;,
 4;126,129,135,133;,
 4;129,52,25,135;,
 4;27,130,135,25;,
 4;130,131,133,135;,
 4;131,132,134,133;,
 3;116,119,136;,
 3;136,119,123;,
 3;117,116,137;,
 3;138,123,117;,
 3;139,79,82;,
 3;80,79,139;,
 3;87,80,140;,
 3;141,82,87;,
 3;142,104,107;,
 3;105,104,142;,
 3;111,105,143;,
 3;144,107,111;,
 3;124,127,145;,
 3;145,127,134;,
 3;132,124,146;,
 3;147,134,132;,
 3;148,96,88;,
 3;149,88,91;,
 3;150,91,98;,
 3;98,96,148;,
 4;151,152,153,154;,
 4;152,155,156,153;,
 4;155,43,22,156;,
 4;43,155,157,45;,
 4;155,152,158,157;,
 4;152,151,159,158;,
 4;154,153,160,161;,
 4;153,156,162,160;,
 4;156,22,30,162;,
 4;45,157,162,30;,
 4;157,158,160,162;,
 4;158,159,161,160;,
 3;163,154,161;,
 3;151,154,163;,
 3;159,151,164;,
 3;165,161,159;,
 4;166,48,51,167;,
 4;168,57,56,169;,
 4;170,84,74,171;,
 4;172,73,72,173;,
 4;174,64,67,175;,
 4;176,58,61,177;,
 3;178,179,180;,
 4;180,166,167,178;,
 3;181,182,183;,
 4;183,168,169,181;,
 3;184,185,186;,
 4;186,170,171,184;,
 3;187,188,189;,
 4;189,172,173,187;,
 3;190,191,192;,
 4;192,174,175,190;,
 3;193,194,195;,
 4;195,176,177,193;;
 
 MeshMaterialList {
  1;
  150;
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
  287;
  0.999382;0.035143;0.000000;,
  0.999036;-0.043894;0.000000;,
  0.498389;0.012551;-0.866863;,
  0.498908;0.066062;-0.864134;,
  0.498908;0.066062;-0.864134;,
  0.498503;0.017803;-0.866705;,
  0.498908;0.066062;0.864134;,
  0.499965;0.011774;0.865965;,
  0.499913;0.018706;0.865874;,
  0.498908;0.066062;0.864134;,
  -0.495430;0.093672;-0.863582;,
  -0.495935;0.127248;-0.858985;,
  -0.858715;0.164079;-0.485475;,
  -0.754443;0.133763;-0.642590;,
  -0.495935;0.127248;0.858985;,
  -0.495544;0.133205;0.858308;,
  -0.840600;0.174128;0.512904;,
  -0.858715;0.164079;0.485475;,
  0.859809;0.203645;0.468249;,
  0.856548;0.208202;0.472205;,
  0.979392;0.190380;0.067436;,
  0.928346;0.241309;0.282742;,
  0.779316;0.199491;0.594029;,
  0.968816;0.244972;-0.037206;,
  0.518445;0.171873;0.837660;,
  0.648975;0.017069;0.760619;,
  0.507980;0.061932;0.859139;,
  0.461388;-0.096513;-0.881933;,
  0.278788;0.081362;-0.956900;,
  0.479034;0.090459;-0.873123;,
  0.462259;-0.110245;-0.879865;,
  -0.489732;-0.042546;0.870834;,
  -0.534274;0.003625;0.845304;,
  -0.636580;-0.023927;0.770839;,
  -0.813098;0.010012;0.582041;,
  -0.517943;-0.164641;0.839421;,
  -0.559153;-0.149963;0.815389;,
  -0.748402;0.124954;-0.651369;,
  -0.657869;0.060981;-0.750660;,
  -0.496439;0.073208;-0.864979;,
  -0.506915;0.099005;-0.856291;,
  -0.589653;0.004055;-0.807646;,
  -0.906370;-0.394872;0.150230;,
  -0.820763;0.041108;0.569787;,
  0.953623;-0.300799;-0.011090;,
  0.954808;-0.297025;-0.010846;,
  0.465435;0.006318;0.885060;,
  0.588537;-0.025407;0.808071;,
  0.380613;0.061727;-0.922672;,
  0.252166;0.041173;-0.966808;,
  -0.483440;-0.255228;0.837343;,
  -0.778189;-0.007274;0.627989;,
  -0.500630;-0.177208;-0.847329;,
  -0.924386;0.157218;0.347552;,
  -0.973655;0.192181;0.122732;,
  0.566180;0.330231;-0.755240;,
  0.525754;0.578339;-0.623784;,
  0.009455;0.398421;-0.917154;,
  0.059561;0.124315;-0.990454;,
  0.268849;0.658091;-0.703304;,
  0.268849;0.658091;-0.703304;,
  0.866842;0.467450;0.173421;,
  0.874899;0.412845;0.253200;,
  0.879272;0.392019;-0.270560;,
  -0.447063;-0.457450;-0.768683;,
  -0.447063;-0.457450;-0.768683;,
  -0.860120;-0.347346;-0.373557;,
  -0.745013;-0.363845;-0.559082;,
  0.446565;-0.290125;0.846408;,
  -0.706964;-0.335712;0.622494;,
  -0.918146;-0.232745;0.320684;,
  -0.556613;-0.250222;0.792194;,
  -0.574475;-0.109630;0.811147;,
  0.787289;0.320574;-0.526696;,
  0.770014;0.340331;-0.539678;,
  0.780237;0.235984;-0.579260;,
  0.795365;0.254870;-0.549941;,
  0.702435;0.241249;-0.669615;,
  0.708490;0.236964;-0.664748;,
  -0.322684;0.016770;-0.946358;,
  -0.247464;0.184839;-0.951103;,
  -0.247708;0.184833;-0.951040;,
  -0.318704;0.050970;-0.946483;,
  -0.286646;0.419116;-0.861497;,
  -0.285654;0.416422;-0.863130;,
  -0.353173;0.611730;-0.707852;,
  -0.353173;0.611730;-0.707853;,
  0.378681;-0.467332;0.798875;,
  0.301264;-0.297765;0.905857;,
  0.284887;-0.263794;0.921549;,
  0.238907;-0.108581;0.964953;,
  0.240587;-0.109860;0.964390;,
  0.280586;0.080041;0.956486;,
  0.284285;0.035737;0.958073;,
  -0.761042;-0.220239;0.610173;,
  -0.828016;-0.204118;0.522231;,
  -0.840973;-0.211276;0.498123;,
  -0.766048;-0.217156;0.604990;,
  -0.827434;-0.239913;0.507735;,
  -0.807287;-0.249514;0.534818;,
  -0.071932;0.128948;0.989039;,
  0.018225;0.129123;0.991461;,
  -0.005743;0.124802;0.992165;,
  -0.072772;0.130594;0.988762;,
  0.041605;0.091644;0.994922;,
  0.031449;0.077734;0.996478;,
  -0.002087;0.031728;0.999494;,
  -0.811281;0.584290;0.020715;,
  -0.610533;0.791912;0.011181;,
  -0.612504;0.790387;0.011268;,
  -0.811281;0.584290;0.020715;,
  -0.547111;0.836182;-0.038327;,
  -0.570303;0.820316;-0.042852;,
  -0.623019;0.776206;-0.096700;,
  -0.620603;0.778108;-0.096952;,
  0.721017;-0.685436;0.101547;,
  0.623370;-0.780974;0.038588;,
  0.633427;-0.772704;0.041221;,
  0.721491;-0.684962;0.101381;,
  0.710103;-0.703990;-0.012287;,
  0.711770;-0.702304;-0.012374;,
  0.004493;0.042300;-0.999095;,
  0.024002;0.052644;-0.998325;,
  -0.006490;0.014996;-0.999866;,
  0.018913;0.018604;-0.999648;,
  0.100539;0.019521;-0.994742;,
  0.104072;0.018611;-0.994396;,
  0.049747;0.098579;0.993885;,
  0.045409;0.098903;0.994061;,
  0.043068;0.110661;0.992925;,
  0.046008;0.108704;0.993009;,
  0.019378;0.126310;0.991802;,
  0.019434;0.126273;0.991805;,
  0.483804;0.874843;0.024175;,
  0.610787;0.791410;0.024693;,
  0.610836;0.791372;0.024693;,
  0.483853;0.874815;0.024175;,
  0.665937;0.745623;0.023963;,
  0.666051;0.745521;0.023965;,
  0.620048;0.784221;0.023195;,
  0.620048;0.784221;0.023195;,
  -0.744570;-0.667467;-0.010182;,
  -0.760984;-0.648679;-0.010895;,
  -0.761045;-0.648608;-0.010898;,
  -0.709770;-0.704335;-0.011797;,
  -0.709561;-0.704546;-0.011798;,
  -0.638181;-0.769795;-0.011837;,
  -0.638199;-0.769780;-0.011837;,
  0.022671;0.124635;-0.991944;,
  0.045300;0.108948;-0.993015;,
  0.048039;0.107048;-0.993093;,
  0.022706;0.124611;-0.991946;,
  0.050905;0.097336;-0.993949;,
  0.046595;0.097755;-0.994119;,
  -0.703965;0.068625;0.706911;,
  -0.633309;0.015181;0.773750;,
  -0.746555;0.107266;0.656619;,
  -0.749522;0.108850;0.652969;,
  -0.767305;0.118505;0.630237;,
  -0.757404;0.113096;0.643078;,
  0.242168;0.855138;0.458359;,
  0.280371;0.822327;0.495147;,
  0.257523;0.842446;0.473251;,
  0.264449;0.836506;0.479921;,
  0.416166;0.618126;0.666878;,
  0.416696;0.620967;0.663901;,
  0.408818;0.581182;0.703630;,
  0.408818;0.581182;0.703630;,
  -0.463096;-0.524005;-0.714815;,
  -0.461695;-0.532808;-0.709192;,
  -0.475141;-0.422860;-0.771642;,
  -0.318383;-0.785392;-0.530842;,
  -0.296409;-0.807288;-0.510322;,
  -0.311177;-0.792743;-0.524145;,
  -0.338499;-0.763972;-0.549331;,
  0.742583;-0.003291;-0.669746;,
  0.752275;-0.008347;-0.658796;,
  0.764482;-0.014851;-0.644474;,
  0.735778;0.000205;-0.677223;,
  0.728847;0.009434;-0.684612;,
  0.649787;0.052916;-0.758272;,
  -0.322092;0.715409;0.620038;,
  -0.285170;0.786803;0.547374;,
  -0.289690;0.779424;0.555498;,
  -0.316908;0.725961;0.610369;,
  -0.345537;0.679687;0.647017;,
  -0.346340;0.678511;0.647821;,
  -0.356720;0.663027;0.658138;,
  -0.356720;0.663027;0.658138;,
  -0.858641;-0.023894;-0.512021;,
  -0.900821;-0.013466;-0.433982;,
  -0.867579;-0.053903;-0.494370;,
  -0.858419;-0.047724;-0.510725;,
  -0.824951;-0.045092;-0.563403;,
  -0.872490;-0.063472;-0.484491;,
  0.838581;0.152701;0.522939;,
  0.866202;0.163673;0.472128;,
  0.856791;0.156860;0.491227;,
  0.874761;0.172871;0.452668;,
  0.843495;0.117469;0.524135;,
  0.864541;0.082111;0.495808;,
  0.419914;-0.475859;-0.772807;,
  0.419196;-0.478373;-0.771644;,
  0.321687;-0.727255;-0.606314;,
  0.324645;-0.721462;-0.611635;,
  0.367611;-0.616122;-0.696603;,
  0.362039;-0.631154;-0.685983;,
  0.409318;0.676747;0.611942;,
  0.757445;-0.011083;-0.652805;,
  -0.787063;0.129577;0.603110;,
  -0.484294;-0.559196;-0.672874;,
  -0.334605;-0.117802;-0.934966;,
  0.650311;0.275911;-0.707791;,
  0.258827;0.265172;0.928813;,
  -0.715126;-0.246690;0.654018;,
  0.472325;0.881095;0.024101;,
  0.013044;0.130430;0.991372;,
  -0.647777;-0.761738;-0.011845;,
  0.016467;0.128950;-0.991514;,
  -0.393368;0.528353;0.752399;,
  0.809482;0.137751;0.570757;,
  -0.785903;-0.031507;-0.617547;,
  0.421567;-0.427802;-0.799541;,
  -0.459989;0.880992;-0.110736;,
  -0.081253;0.147202;0.985763;,
  0.656739;-0.744074;0.122673;,
  0.133596;0.010987;-0.990975;,
  0.897200;-0.158422;0.412231;,
  0.867096;-0.145299;0.476480;,
  0.872139;-0.147417;0.466522;,
  0.888499;-0.154500;0.432088;,
  0.762486;-0.105841;0.638289;,
  0.782517;-0.112896;0.612308;,
  -0.352637;-0.708935;0.610785;,
  -0.353180;-0.707854;0.611725;,
  -0.136732;-0.961882;0.236826;,
  -0.158712;-0.948284;0.274898;,
  -0.214411;-0.903389;0.371370;,
  -0.161270;-0.946557;0.279326;,
  0.140634;0.959630;-0.243583;,
  0.100900;0.979427;-0.174764;,
  0.120980;0.970286;-0.209544;,
  0.097630;0.980752;-0.169099;,
  0.323781;0.762013;-0.560805;,
  0.323114;0.763145;-0.559649;,
  0.369172;0.674425;-0.639424;,
  0.369172;0.674425;-0.639424;,
  -0.750946;0.200679;-0.629133;,
  -0.760807;0.204386;-0.615954;,
  -0.770646;0.208109;-0.602325;,
  -0.750946;0.200679;-0.629133;,
  -0.862756;0.244571;-0.442535;,
  -0.870963;0.248027;-0.424152;,
  -0.911963;0.266301;-0.312102;,
  -0.905924;0.263465;-0.331495;,
  0.302987;0.795487;-0.524785;,
  0.849091;-0.137948;0.509916;,
  -0.374611;-0.662321;0.648843;,
  -0.883075;0.253224;-0.395039;,
  0.470702;-0.032029;-0.881711;,
  -0.126278;0.098585;-0.987084;,
  0.863414;-0.052685;0.501738;,
  0.990097;0.137053;-0.030392;,
  0.999925;-0.009583;-0.007670;,
  0.295007;-0.012257;0.955417;,
  0.537990;0.054381;0.841195;,
  0.537990;0.054381;0.841195;,
  -0.464638;0.036511;0.884748;,
  -0.435961;-0.039335;0.899105;,
  -0.889482;0.104148;-0.444943;,
  -0.999159;-0.040773;0.004253;,
  -0.988132;0.151609;-0.024698;,
  0.487355;-0.074843;-0.869990;,
  0.458637;0.016618;-0.888468;,
  0.472811;-0.026763;-0.880757;,
  0.967862;-0.251301;0.009522;,
  0.861397;-0.308255;0.403701;,
  0.870533;-0.255787;0.420410;,
  0.585798;-0.150160;0.796425;,
  -0.499910;0.134949;0.855499;,
  -0.488383;0.094214;0.867528;,
  -0.511468;0.179443;0.840357;,
  -0.520136;0.215988;0.826322;,
  -0.944776;0.326530;-0.027869;,
  -0.832691;0.386125;-0.396904;,
  -0.834589;0.301007;-0.461363;,
  -0.582411;0.278797;-0.763590;;
  150;
  4;0,1,1,0;,
  4;2,3,4,5;,
  4;6,7,8,9;,
  4;10,11,12,13;,
  4;14,15,16,17;,
  4;13,12,17,16;,
  4;18,19,20,21;,
  4;19,22,23,20;,
  4;22,19,24,25;,
  4;19,18,26,24;,
  4;27,28,29,30;,
  4;31,32,33,34;,
  4;32,35,36,33;,
  4;37,38,39,40;,
  4;38,41,41,39;,
  4;34,33,42,43;,
  4;44,1,1,45;,
  4;7,46,47,8;,
  4;46,22,25,47;,
  4;28,48,49,29;,
  4;48,2,5,49;,
  4;15,50,51,16;,
  4;49,10,13,52;,
  4;16,51,53,54;,
  4;51,34,43,53;,
  4;55,56,57,58;,
  4;56,59,60,57;,
  4;21,61,62,18;,
  4;56,55,63,62;,
  4;64,65,66,67;,
  4;57,37,40,66;,
  4;18,62,68,26;,
  4;67,66,69,70;,
  4;66,40,36,69;,
  4;35,71,69,36;,
  4;71,72,70,69;,
  4;63,73,74,62;,
  4;73,75,76,74;,
  4;75,77,78,76;,
  4;79,80,81,82;,
  4;80,83,84,81;,
  4;83,85,86,84;,
  4;87,88,89,68;,
  4;88,90,91,89;,
  4;90,92,93,91;,
  4;94,95,96,97;,
  4;95,98,99,96;,
  4;98,72,71,99;,
  4;100,101,102,103;,
  4;101,104,105,102;,
  4;104,106,46,105;,
  4;107,108,109,110;,
  4;108,111,112,109;,
  4;111,113,114,112;,
  4;115,116,117,118;,
  4;116,119,120,117;,
  4;119,44,45,120;,
  4;28,121,122,48;,
  4;121,123,124,122;,
  4;123,125,126,124;,
  4;36,127,128,33;,
  4;127,129,130,128;,
  4;129,131,132,130;,
  4;133,134,135,136;,
  4;134,137,138,135;,
  4;137,139,140,138;,
  4;141,142,143,42;,
  4;142,144,145,143;,
  4;144,146,147,145;,
  4;148,149,150,151;,
  4;149,152,153,150;,
  4;152,40,39,153;,
  4;43,154,155,53;,
  4;154,156,157,155;,
  4;156,158,159,157;,
  4;160,161,162,163;,
  4;161,164,165,162;,
  4;164,166,167,165;,
  4;52,168,169,170;,
  4;168,171,172,169;,
  4;171,173,174,172;,
  4;175,176,177,178;,
  4;176,179,180,177;,
  4;179,29,49,180;,
  4;181,182,183,184;,
  4;182,185,186,183;,
  4;185,187,188,186;,
  4;37,189,190,38;,
  4;189,191,192,190;,
  4;191,193,194,192;,
  4;195,196,197,198;,
  4;196,199,200,197;,
  4;199,21,20,200;,
  4;30,201,202,27;,
  4;201,203,204,202;,
  4;203,205,206,204;,
  3;160,163,207;,
  3;208,175,178;,
  3;159,158,209;,
  3;210,174,173;,
  3;211,79,82;,
  3;78,77,212;,
  3;93,92,213;,
  3;214,94,97;,
  3;215,133,136;,
  3;132,131,216;,
  3;147,146,217;,
  3;218,148,151;,
  3;181,184,219;,
  3;220,195,198;,
  3;194,193,221;,
  3;222,206,205;,
  3;223,114,113;,
  3;224,100,103;,
  3;225,115,118;,
  3;126,125,226;,
  4;227,228,229,230;,
  4;228,231,232,229;,
  4;231,47,25,232;,
  4;50,233,234,51;,
  4;233,235,236,234;,
  4;235,237,238,236;,
  4;239,240,241,242;,
  4;240,243,244,241;,
  4;243,245,246,244;,
  4;247,248,249,250;,
  4;248,251,252,249;,
  4;251,253,254,252;,
  3;255,239,242;,
  3;227,230,256;,
  3;238,237,257;,
  3;258,254,253;,
  4;259,55,58,260;,
  4;261,63,262,263;,
  4;264,265,266,261;,
  4;267,268,72,264;,
  4;269,270,70,271;,
  4;260,58,67,269;,
  3;272,273,274;,
  4;274,259,260,272;,
  3;275,276,277;,
  4;277,261,263,275;,
  3;277,276,278;,
  4;278,264,261,277;,
  3;279,280,281;,
  4;281,267,282,279;,
  3;283,284,285;,
  4;285,269,271,283;,
  3;285,284,286;,
  4;286,260,269,285;;
 }
 MeshTextureCoords {
  196;
  0.166667;1.000000;,
  0.166667;0.500000;,
  0.333333;0.500000;,
  0.333333;1.000000;,
  0.000000;1.000000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.500000;1.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.833333;1.000000;,
  0.833333;0.500000;,
  0.666667;0.500000;,
  0.666667;1.000000;,
  0.301972;0.000000;,
  0.310726;0.000000;,
  0.189346;0.000000;,
  0.198127;0.000000;,
  0.333333;0.000000;,
  0.166667;0.000000;,
  0.355941;0.000000;,
  0.477429;0.000000;,
  0.500000;0.000000;,
  0.364694;0.000000;,
  0.468689;0.000000;,
  0.143988;0.000000;,
  0.000000;0.000000;,
  0.022715;0.000000;,
  0.522571;0.000000;,
  0.644059;0.000000;,
  0.666667;0.000000;,
  0.531311;0.000000;,
  0.635306;0.000000;,
  0.968490;0.000000;,
  0.977285;0.000000;,
  0.856012;0.000000;,
  0.864794;0.000000;,
  1.000000;0.000000;,
  0.833333;0.000000;,
  0.689274;0.000000;,
  0.810654;0.000000;,
  0.333333;0.140482;,
  0.166667;0.140482;,
  0.500000;0.140482;,
  0.000000;0.140482;,
  0.666667;0.140482;,
  1.000000;0.140482;,
  0.833333;0.140482;,
  0.106147;0.000000;,
  0.113362;0.000000;,
  0.053321;0.000000;,
  0.060526;0.000000;,
  0.135206;0.000000;,
  0.031510;0.000000;,
  0.219971;0.000000;,
  0.280067;0.000000;,
  0.227187;0.000000;,
  0.272831;0.000000;,
  0.939474;0.000000;,
  0.946679;0.000000;,
  0.886633;0.000000;,
  0.893847;0.000000;,
  0.386600;0.000000;,
  0.446755;0.000000;,
  0.772820;0.000000;,
  0.780034;0.000000;,
  0.719928;0.000000;,
  0.727163;0.000000;,
  0.801873;0.000000;,
  0.698028;0.000000;,
  0.553245;0.000000;,
  0.613405;0.000000;,
  0.560491;0.000000;,
  0.606170;0.000000;,
  0.393836;0.000000;,
  0.393836;0.000000;,
  0.386600;0.000000;,
  0.393836;0.000000;,
  0.386600;0.000000;,
  0.393836;0.000000;,
  0.386600;0.000000;,
  0.439509;0.000000;,
  0.439509;0.000000;,
  0.439509;0.000000;,
  0.439509;0.000000;,
  0.446755;0.000000;,
  0.446755;0.000000;,
  0.446755;0.000000;,
  0.333333;0.000000;,
  0.333333;0.000000;,
  0.333333;0.140482;,
  0.333333;0.140482;,
  0.333333;0.000000;,
  0.333333;0.140482;,
  0.166667;0.000000;,
  0.166667;0.000000;,
  0.166667;0.000000;,
  0.166667;0.140482;,
  0.166667;0.140482;,
  0.166667;0.140482;,
  0.698028;0.000000;,
  0.689274;0.000000;,
  0.698028;0.000000;,
  0.689274;0.000000;,
  0.698028;0.000000;,
  0.689274;0.000000;,
  0.801873;0.000000;,
  0.801873;0.000000;,
  0.801873;0.000000;,
  0.810654;0.000000;,
  0.810654;0.000000;,
  0.810654;0.000000;,
  0.833333;0.000000;,
  0.833333;0.140482;,
  0.833333;0.000000;,
  0.833333;0.140482;,
  0.833333;0.000000;,
  0.833333;0.140482;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.140482;,
  1.000000;0.140482;,
  1.000000;0.140482;,
  0.031510;0.000000;,
  0.031510;0.000000;,
  0.135206;0.000000;,
  0.135206;0.000000;,
  0.031510;0.000000;,
  0.135206;0.000000;,
  0.022715;0.000000;,
  0.022715;0.000000;,
  0.022715;0.000000;,
  0.143988;0.000000;,
  0.143988;0.000000;,
  0.143988;0.000000;,
  1.000000;0.000000;,
  0.833333;0.000000;,
  1.000000;0.140482;,
  0.393836;0.000000;,
  0.386600;0.000000;,
  0.439509;0.000000;,
  0.698028;0.000000;,
  0.689274;0.000000;,
  0.801873;0.000000;,
  0.135206;0.000000;,
  0.031510;0.000000;,
  0.143988;0.000000;,
  0.166667;0.000000;,
  0.333333;0.000000;,
  0.333333;0.140482;,
  0.500000;0.140482;,
  0.500000;0.140482;,
  0.500000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.140482;,
  0.500000;0.000000;,
  0.666667;0.140482;,
  0.666667;0.140482;,
  0.666667;0.140482;,
  0.666667;0.000000;,
  0.666667;0.000000;,
  0.666667;0.000000;,
  0.500000;0.000000;,
  0.500000;0.140482;,
  0.666667;0.000000;,
  0.101109;0.000000;,
  0.065381;0.000000;,
  0.267781;0.000000;,
  0.232224;0.000000;,
  0.434630;0.000000;,
  0.398886;0.000000;,
  0.601477;0.000000;,
  0.565370;0.000000;,
  0.768139;0.000000;,
  0.731856;0.000000;,
  0.934619;0.000000;,
  0.898528;0.000000;,
  0.073509;0.000000;,
  0.083333;0.000000;,
  0.093172;0.000000;,
  0.240161;0.000000;,
  0.250000;0.000000;,
  0.259961;0.000000;,
  0.406706;0.000000;,
  0.416667;0.000000;,
  0.426737;0.000000;,
  0.573263;0.000000;,
  0.583333;0.000000;,
  0.593388;0.000000;,
  0.739945;0.000000;,
  0.750000;0.000000;,
  0.759931;0.000000;,
  0.906736;0.000000;,
  0.916667;0.000000;,
  0.926491;0.000000;;
 }
}