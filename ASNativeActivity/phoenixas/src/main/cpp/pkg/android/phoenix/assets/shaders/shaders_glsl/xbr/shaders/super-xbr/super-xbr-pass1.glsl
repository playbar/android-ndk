// GLSL shader autogenerated by cg2glsl.py.
#if defined(VERTEX)

#if __VERSION__ >= 130
#define COMPAT_VARYING out
#define COMPAT_ATTRIBUTE in
#define COMPAT_TEXTURE texture
#else
#define COMPAT_VARYING varying
#define COMPAT_ATTRIBUTE attribute
#define COMPAT_TEXTURE texture2D
#endif

#ifdef GL_ES
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif
COMPAT_VARYING     vec2 _texCoord1;
COMPAT_VARYING     vec4 _color1;
COMPAT_VARYING     vec4 _position1;
COMPAT_VARYING     float _frame_rotation;
struct prev {
    vec2 _video_size;
    vec2 _texture_size;
float _placeholder23;
};
struct input_dummy {
    vec2 _video_size1;
    vec2 _texture_size1;
    vec2 _output_dummy_size;
    float _frame_count;
    float _frame_direction;
    float _frame_rotation;
};
struct out_vertex {
    vec4 _position1;
    vec4 _color1;
    vec2 _texCoord1;
};
out_vertex _ret_0;
vec4 _r0009;
COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_ATTRIBUTE vec4 COLOR;
COMPAT_ATTRIBUTE vec4 TexCoord;
COMPAT_VARYING vec4 COL0;
COMPAT_VARYING vec4 TEX0;
 
uniform mat4 MVPMatrix;
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    _r0009 = VertexCoord.x*MVPMatrix[0];
    _r0009 = _r0009 + VertexCoord.y*MVPMatrix[1];
    _r0009 = _r0009 + VertexCoord.z*MVPMatrix[2];
    _r0009 = _r0009 + VertexCoord.w*MVPMatrix[3];
    _ret_0._position1 = _r0009;
    _ret_0._color1 = COLOR;
    _ret_0._texCoord1 = TexCoord.xy;
    gl_Position = _r0009;
    COL0 = COLOR;
    TEX0.xy = TexCoord.xy;
    return;
    COL0 = _ret_0._color1;
    TEX0.xy = _ret_0._texCoord1;
} 
#elif defined(FRAGMENT)

#if __VERSION__ >= 130
#define COMPAT_VARYING in
#define COMPAT_TEXTURE texture
out vec4 FragColor;
#else
#define COMPAT_VARYING varying
#define FragColor gl_FragColor
#define COMPAT_TEXTURE texture2D
#endif

#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif
COMPAT_VARYING     vec2 _texCoord;
COMPAT_VARYING     vec4 _color2;
COMPAT_VARYING     float _frame_rotation;
struct prev {
    vec2 _video_size;
    vec2 _texture_size;
float _placeholder29;
};
struct input_dummy {
    vec2 _video_size1;
    vec2 _texture_size1;
    vec2 _output_dummy_size;
    float _frame_count;
    float _frame_direction;
    float _frame_rotation;
};
struct out_vertex {
    vec4 _color2;
    vec2 _texCoord;
};
vec4 _ret_0;
vec3 _TMP61;
vec3 _TMP33;
float _TMP32;
vec3 _TMP60;
vec3 _TMP59;
vec3 _TMP30;
float _TMP29;
vec3 _TMP58;
vec3 _TMP57;
vec3 _TMP27;
vec3 _TMP25;
float _TMP26;
float _TMP24;
float _TMP23;
float _TMP62;
float _TMP22;
float _TMP21;
float _TMP20;
float _TMP19;
vec4 _TMP18;
vec4 _TMP17;
vec4 _TMP16;
vec4 _TMP15;
vec4 _TMP14;
vec4 _TMP13;
vec4 _TMP12;
vec4 _TMP11;
vec4 _TMP10;
vec4 _TMP9;
vec4 _TMP8;
vec4 _TMP7;
vec4 _TMP6;
vec4 _TMP5;
vec4 _TMP4;
vec4 _TMP3;
vec2 _TMP2;
vec2 _TMP1;
vec4 _TMP0;
uniform sampler2D Texture;
prev _PASSPREV21;
input_dummy _IN1;
vec2 _x0073;
vec2 _c0079;
vec2 _c0081;
vec2 _c0083;
vec2 _c0085;
vec2 _c0087;
vec2 _c0089;
vec2 _c0091;
vec2 _c0093;
vec2 _c0095;
vec2 _c0097;
vec2 _c0099;
vec2 _c0101;
vec2 _c0103;
vec2 _c0105;
vec2 _c0107;
vec2 _c0109;
float _TMP110;
float _TMP114;
float _TMP118;
float _TMP122;
float _TMP126;
float _TMP130;
float _TMP134;
float _TMP138;
float _TMP142;
float _TMP150;
float _TMP158;
float _TMP166;
float _TMP176;
float _a0179;
float _TMP180;
float _a0183;
float _TMP184;
float _a0187;
float _TMP188;
float _a0191;
float _TMP208;
float _a0211;
float _TMP230;
float _a0233;
float _TMP234;
float _a0237;
float _TMP238;
float _a0241;
float _TMP242;
float _a0245;
float _TMP262;
float _a0265;
float _TMP284;
float _a0287;
float _TMP288;
float _a0291;
float _TMP292;
float _a0295;
float _TMP296;
float _a0299;
float _TMP300;
float _a0303;
float _TMP304;
float _a0307;
float _TMP326;
float _a0329;
float _TMP330;
float _a0333;
float _TMP334;
float _a0337;
float _TMP338;
float _a0341;
float _TMP342;
float _a0345;
float _TMP346;
float _a0349;
float _x0371;
float _TMP372;
vec3 _r0379;
vec3 _r0381;
vec3 _r0383;
vec3 _r0385;
float _t0395;
vec3 _TMP396;
vec3 _a0407;
vec3 _b0407;
vec3 _TMP408;
vec3 _a0419;
vec3 _b0419;
vec3 _TMP420;
COMPAT_VARYING vec4 TEX0;
 
uniform sampler2D PassPrev2Texture;
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    vec2 _fp;
    vec2 _dir;
    float _d_edge;
    float _hv_edge;
    float _edge_strength;
    vec3 _color1;
    vec3 _min_sample;
    vec3 _max_sample;
    vec3 _TMP68[4];
    vec3 _TMP69[4];
    _x0073 = TEX0.xy*TextureSize;
    _fp = fract(_x0073);
    _dir = _fp - vec2( 5.00000000E-01, 5.00000000E-01);
    if (_dir.x*_dir.y > 0.00000000E+00) { 
        if (_fp.x > 5.00000000E-01) { 
            _TMP0 = COMPAT_TEXTURE(Texture, TEX0.xy);
        } else {
            _TMP0 = COMPAT_TEXTURE(PassPrev2Texture, TEX0.xy);
        } 
        FragColor = _TMP0;
        return;
    } 
    if (_fp.x > 5.00000000E-01) { 
        _TMP1 = vec2(5.00000000E-01/TextureSize.x, 0.00000000E+00);
    } else {
        _TMP1 = vec2(0.00000000E+00, 5.00000000E-01/TextureSize.y);
    } 
    if (_fp.x > 5.00000000E-01) { 
        _TMP2 = vec2(0.00000000E+00, 5.00000000E-01/TextureSize.y);
    } else {
        _TMP2 = vec2(5.00000000E-01/TextureSize.x, 0.00000000E+00);
    } 
    _c0079 = TEX0.xy - 3.00000000E+00*_TMP1;
    _TMP3 = COMPAT_TEXTURE(PassPrev2Texture, _c0079);
    _c0081 = TEX0.xy - 3.00000000E+00*_TMP2;
    _TMP4 = COMPAT_TEXTURE(Texture, _c0081);
    _c0083 = TEX0.xy + 3.00000000E+00*_TMP2;
    _TMP5 = COMPAT_TEXTURE(Texture, _c0083);
    _c0085 = TEX0.xy + 3.00000000E+00*_TMP1;
    _TMP6 = COMPAT_TEXTURE(PassPrev2Texture, _c0085);
    _c0087 = (TEX0.xy - 2.00000000E+00*_TMP1) - _TMP2;
    _TMP7 = COMPAT_TEXTURE(Texture, _c0087);
    _c0089 = (TEX0.xy - _TMP1) - 2.00000000E+00*_TMP2;
    _TMP8 = COMPAT_TEXTURE(PassPrev2Texture, _c0089);
    _c0091 = (TEX0.xy - 2.00000000E+00*_TMP1) + _TMP2;
    _TMP9 = COMPAT_TEXTURE(Texture, _c0091);
    _c0093 = TEX0.xy - _TMP1;
    _TMP10 = COMPAT_TEXTURE(PassPrev2Texture, _c0093);
    _c0095 = TEX0.xy - _TMP2;
    _TMP11 = COMPAT_TEXTURE(Texture, _c0095);
    _c0097 = (TEX0.xy - _TMP1) + 2.00000000E+00*_TMP2;
    _TMP12 = COMPAT_TEXTURE(PassPrev2Texture, _c0097);
    _c0099 = TEX0.xy + _TMP2;
    _TMP13 = COMPAT_TEXTURE(Texture, _c0099);
    _c0101 = TEX0.xy + _TMP1;
    _TMP14 = COMPAT_TEXTURE(PassPrev2Texture, _c0101);
    _c0103 = (TEX0.xy + _TMP1) - 2.00000000E+00*_TMP2;
    _TMP15 = COMPAT_TEXTURE(PassPrev2Texture, _c0103);
    _c0105 = (TEX0.xy + 2.00000000E+00*_TMP1) - _TMP2;
    _TMP16 = COMPAT_TEXTURE(Texture, _c0105);
    _c0107 = TEX0.xy + _TMP1 + 2.00000000E+00*_TMP2;
    _TMP17 = COMPAT_TEXTURE(PassPrev2Texture, _c0107);
    _c0109 = TEX0.xy + 2.00000000E+00*_TMP1 + _TMP2;
    _TMP18 = COMPAT_TEXTURE(Texture, _c0109);
    _TMP110 = dot(_TMP7.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP114 = dot(_TMP8.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP118 = dot(_TMP9.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP122 = dot(_TMP10.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP126 = dot(_TMP11.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP130 = dot(_TMP12.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP134 = dot(_TMP13.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP138 = dot(_TMP14.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP142 = dot(_TMP16.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP150 = dot(_TMP18.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP158 = dot(_TMP17.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP166 = dot(_TMP15.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _a0179 = _TMP122 - _TMP114;
    _TMP176 = abs(_a0179);
    _a0183 = _TMP122 - _TMP130;
    _TMP180 = abs(_a0183);
    _a0187 = _TMP138 - _TMP158;
    _TMP184 = abs(_a0187);
    _a0191 = _TMP138 - _TMP166;
    _TMP188 = abs(_a0191);
    _a0211 = _TMP134 - _TMP126;
    _TMP208 = abs(_a0211);
    _TMP19 = _TMP176 + _TMP180 + _TMP184 + _TMP188 + 4.00000000E+00*_TMP208;
    _a0233 = _TMP126 - _TMP142;
    _TMP230 = abs(_a0233);
    _a0237 = _TMP126 - _TMP110;
    _TMP234 = abs(_a0237);
    _a0241 = _TMP134 - _TMP118;
    _TMP238 = abs(_a0241);
    _a0245 = _TMP134 - _TMP150;
    _TMP242 = abs(_a0245);
    _a0265 = _TMP122 - _TMP138;
    _TMP262 = abs(_a0265);
    _TMP20 = _TMP230 + _TMP234 + _TMP238 + _TMP242 + 4.00000000E+00*_TMP262;
    _d_edge = _TMP19 - _TMP20;
    _a0287 = _TMP126 - _TMP138;
    _TMP284 = abs(_a0287);
    _a0291 = _TMP122 - _TMP134;
    _TMP288 = abs(_a0291);
    _a0295 = _TMP126 - _TMP114;
    _TMP292 = abs(_a0295);
    _a0299 = _TMP138 - _TMP150;
    _TMP296 = abs(_a0299);
    _a0303 = _TMP122 - _TMP110;
    _TMP300 = abs(_a0303);
    _a0307 = _TMP134 - _TMP158;
    _TMP304 = abs(_a0307);
    _TMP21 = 4.00000000E+00*(_TMP284 + _TMP288) + _TMP292 + _TMP296 + _TMP300 + _TMP304;
    _a0329 = _TMP122 - _TMP126;
    _TMP326 = abs(_a0329);
    _a0333 = _TMP134 - _TMP138;
    _TMP330 = abs(_a0333);
    _a0337 = _TMP122 - _TMP118;
    _TMP334 = abs(_a0337);
    _a0341 = _TMP126 - _TMP166;
    _TMP338 = abs(_a0341);
    _a0345 = _TMP134 - _TMP130;
    _TMP342 = abs(_a0345);
    _a0349 = _TMP138 - _TMP142;
    _TMP346 = abs(_a0349);
    _TMP22 = 4.00000000E+00*(_TMP326 + _TMP330) + _TMP334 + _TMP338 + _TMP342 + _TMP346;
    _hv_edge = _TMP21 - _TMP22;
    _TMP23 = abs(_d_edge);
    _x0371 = _TMP23/9.99999997E-07;
    _TMP62 = min(1.00000000E+00, _x0371);
    _TMP372 = max(0.00000000E+00, _TMP62);
    _edge_strength = _TMP372*_TMP372*(3.00000000E+00 - 2.00000000E+00*_TMP372);
    _r0379 = 5.00000000E-01*_TMP13.xyz;
    _r0379 = _r0379 + 5.00000000E-01*_TMP11.xyz;
    _r0381 = 5.00000000E-01*_TMP10.xyz;
    _r0381 = _r0381 + 5.00000000E-01*_TMP14.xyz;
    _TMP69[1] = _TMP10.xyz + _TMP13.xyz;
    _TMP69[2] = _TMP11.xyz + _TMP14.xyz;
    _r0383 = 2.50000000E-01*_TMP69[1];
    _r0383 = _r0383 + 2.50000000E-01*_TMP69[2];
    _TMP68[1] = _TMP11.xyz + _TMP10.xyz;
    _TMP68[2] = _TMP14.xyz + _TMP13.xyz;
    _r0385 = 2.50000000E-01*_TMP68[1];
    _r0385 = _r0385 + 2.50000000E-01*_TMP68[2];
    _TMP24 = float((_d_edge >= 0.00000000E+00));
    _TMP25 = _r0379 + _TMP24*(_r0381 - _r0379);
    _TMP26 = float((_hv_edge >= 0.00000000E+00));
    _TMP27 = _r0383 + _TMP26*(_r0385 - _r0383);
    _t0395 = 1.00000000E+00 - _edge_strength;
    _color1 = _TMP25 + _t0395*(_TMP27 - _TMP25);
    _TMP57 = min(_TMP13.xyz, _TMP14.xyz);
    _TMP58 = min(_TMP11.xyz, _TMP57);
    _TMP396 = min(_TMP10.xyz, _TMP58);
    _TMP29 = float((_d_edge >= 0.00000000E+00));
    _a0407 = (_TMP5.xyz - _TMP13.xyz)*(_TMP11.xyz - _TMP4.xyz);
    _b0407 = (_TMP3.xyz - _TMP10.xyz)*(_TMP14.xyz - _TMP6.xyz);
    _TMP30 = _a0407 + _TMP29*(_b0407 - _a0407);
    _min_sample = _TMP396 + _TMP30;
    _TMP59 = max(_TMP13.xyz, _TMP14.xyz);
    _TMP60 = max(_TMP11.xyz, _TMP59);
    _TMP408 = max(_TMP10.xyz, _TMP60);
    _TMP32 = float((_d_edge >= 0.00000000E+00));
    _a0419 = (_TMP5.xyz - _TMP13.xyz)*(_TMP11.xyz - _TMP4.xyz);
    _b0419 = (_TMP3.xyz - _TMP10.xyz)*(_TMP14.xyz - _TMP6.xyz);
    _TMP33 = _a0419 + _TMP32*(_b0419 - _a0419);
    _max_sample = _TMP408 - _TMP33;
    _TMP61 = min(_max_sample, _color1);
    _TMP420 = max(_min_sample, _TMP61);
    _ret_0 = vec4(_TMP420.x, _TMP420.y, _TMP420.z, 1.00000000E+00);
    FragColor = _ret_0;
    return;
} 
#endif
