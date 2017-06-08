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
COMPAT_VARYING     vec2 _texCoord;
COMPAT_VARYING     vec4 _color1;
COMPAT_VARYING     vec4 _position1;
COMPAT_VARYING     float _frame_rotation;
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
    vec2 _output_dummy_size;
    float _frame_count;
    float _frame_direction;
    float _frame_rotation;
};
struct out_vertex {
    vec4 _position1;
    vec4 _color1;
    vec2 _texCoord;
};
out_vertex _ret_0;
vec4 _r0008;
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
    _r0008 = VertexCoord.x*MVPMatrix[0];
    _r0008 = _r0008 + VertexCoord.y*MVPMatrix[1];
    _r0008 = _r0008 + VertexCoord.z*MVPMatrix[2];
    _r0008 = _r0008 + VertexCoord.w*MVPMatrix[3];
    _ret_0._position1 = _r0008;
    _ret_0._color1 = COLOR;
    _ret_0._texCoord = TexCoord.xy;
    gl_Position = _r0008;
    COL0 = COLOR;
    TEX0.xy = TexCoord.xy;
    return;
    COL0 = _ret_0._color1;
    TEX0.xy = _ret_0._texCoord;
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
struct input_dummy {
    vec2 _video_size;
    vec2 _texture_size;
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
vec3 _TMP42;
vec3 _TMP24;
float _TMP23;
vec3 _TMP41;
vec3 _TMP40;
vec3 _TMP21;
float _TMP20;
vec3 _TMP39;
vec3 _TMP38;
float _TMP43;
float _TMP18;
float _TMP17;
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
vec4 _TMP2;
vec4 _TMP1;
vec2 _TMP0;
uniform sampler2D Texture;
input_dummy _IN1;
vec2 _x0051;
vec2 _c0055;
vec2 _c0057;
vec2 _c0059;
vec2 _c0061;
vec2 _c0063;
vec2 _c0065;
vec2 _c0067;
vec2 _c0069;
vec2 _c0071;
vec2 _c0073;
vec2 _c0075;
vec2 _c0077;
vec2 _c0079;
vec2 _c0081;
vec2 _c0083;
vec2 _c0085;
float _TMP86;
float _TMP90;
float _TMP94;
float _TMP98;
float _TMP102;
float _TMP106;
float _TMP110;
float _TMP114;
float _TMP118;
float _TMP126;
float _TMP134;
float _TMP142;
float _TMP152;
float _a0155;
float _TMP156;
float _a0159;
float _TMP160;
float _a0163;
float _TMP164;
float _a0167;
float _TMP184;
float _a0187;
float _TMP206;
float _a0209;
float _TMP210;
float _a0213;
float _TMP214;
float _a0217;
float _TMP218;
float _a0221;
float _TMP238;
float _a0241;
float _x0261;
float _TMP262;
vec3 _r0269;
vec3 _r0271;
vec3 _TMP274;
vec3 _a0285;
vec3 _b0285;
vec3 _TMP286;
vec3 _a0297;
vec3 _b0297;
vec3 _TMP298;
COMPAT_VARYING vec4 TEX0;
 
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    vec2 _dir;
    vec2 _g1;
    vec2 _g2;
    float _d_edge;
    float _edge_strength;
    vec3 _color1;
    vec3 _min_sample;
    vec3 _max_sample;
    _x0051 = (TEX0.xy*TextureSize)/2.00000000E+00;
    _TMP0 = fract(_x0051);
    _dir = _TMP0 - vec2( 5.00000000E-01, 5.00000000E-01);
    if (_dir.x*_dir.y > 0.00000000E+00) { 
        _ret_0 = COMPAT_TEXTURE(Texture, TEX0.xy);
        FragColor = _ret_0;
        return;
    } 
    _g1 = vec2(1.00000000E+00/TextureSize.x, 0.00000000E+00);
    _g2 = vec2(0.00000000E+00, 1.00000000E+00/TextureSize.y);
    _c0055 = TEX0.xy - 3.00000000E+00*_g1;
    _TMP1 = COMPAT_TEXTURE(Texture, _c0055);
    _c0057 = TEX0.xy - 3.00000000E+00*_g2;
    _TMP2 = COMPAT_TEXTURE(Texture, _c0057);
    _c0059 = TEX0.xy + 3.00000000E+00*_g2;
    _TMP3 = COMPAT_TEXTURE(Texture, _c0059);
    _c0061 = TEX0.xy + 3.00000000E+00*_g1;
    _TMP4 = COMPAT_TEXTURE(Texture, _c0061);
    _c0063 = (TEX0.xy - 2.00000000E+00*_g1) - _g2;
    _TMP5 = COMPAT_TEXTURE(Texture, _c0063);
    _c0065 = (TEX0.xy - _g1) - 2.00000000E+00*_g2;
    _TMP6 = COMPAT_TEXTURE(Texture, _c0065);
    _c0067 = (TEX0.xy - 2.00000000E+00*_g1) + _g2;
    _TMP7 = COMPAT_TEXTURE(Texture, _c0067);
    _c0069 = TEX0.xy - _g1;
    _TMP8 = COMPAT_TEXTURE(Texture, _c0069);
    _c0071 = TEX0.xy - _g2;
    _TMP9 = COMPAT_TEXTURE(Texture, _c0071);
    _c0073 = (TEX0.xy - _g1) + 2.00000000E+00*_g2;
    _TMP10 = COMPAT_TEXTURE(Texture, _c0073);
    _c0075 = TEX0.xy + _g2;
    _TMP11 = COMPAT_TEXTURE(Texture, _c0075);
    _c0077 = TEX0.xy + _g1;
    _TMP12 = COMPAT_TEXTURE(Texture, _c0077);
    _c0079 = (TEX0.xy + _g1) - 2.00000000E+00*_g2;
    _TMP13 = COMPAT_TEXTURE(Texture, _c0079);
    _c0081 = (TEX0.xy + 2.00000000E+00*_g1) - _g2;
    _TMP14 = COMPAT_TEXTURE(Texture, _c0081);
    _c0083 = TEX0.xy + _g1 + 2.00000000E+00*_g2;
    _TMP15 = COMPAT_TEXTURE(Texture, _c0083);
    _c0085 = TEX0.xy + 2.00000000E+00*_g1 + _g2;
    _TMP16 = COMPAT_TEXTURE(Texture, _c0085);
    _TMP86 = dot(_TMP5.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP90 = dot(_TMP6.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP94 = dot(_TMP7.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP98 = dot(_TMP8.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP102 = dot(_TMP9.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP106 = dot(_TMP10.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP110 = dot(_TMP11.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP114 = dot(_TMP12.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP118 = dot(_TMP14.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP126 = dot(_TMP16.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP134 = dot(_TMP15.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _TMP142 = dot(_TMP13.xyz, vec3( 2.12599993E-01, 7.15200007E-01, 7.22000003E-02));
    _a0155 = _TMP98 - _TMP90;
    _TMP152 = abs(_a0155);
    _a0159 = _TMP98 - _TMP106;
    _TMP156 = abs(_a0159);
    _a0163 = _TMP114 - _TMP134;
    _TMP160 = abs(_a0163);
    _a0167 = _TMP114 - _TMP142;
    _TMP164 = abs(_a0167);
    _a0187 = _TMP110 - _TMP102;
    _TMP184 = abs(_a0187);
    _TMP17 = _TMP152 + _TMP156 + _TMP160 + _TMP164 + 4.00000000E+00*_TMP184;
    _a0209 = _TMP102 - _TMP118;
    _TMP206 = abs(_a0209);
    _a0213 = _TMP102 - _TMP86;
    _TMP210 = abs(_a0213);
    _a0217 = _TMP110 - _TMP94;
    _TMP214 = abs(_a0217);
    _a0221 = _TMP110 - _TMP126;
    _TMP218 = abs(_a0221);
    _a0241 = _TMP98 - _TMP114;
    _TMP238 = abs(_a0241);
    _TMP18 = _TMP206 + _TMP210 + _TMP214 + _TMP218 + 4.00000000E+00*_TMP238;
    _d_edge = _TMP17 - _TMP18;
    _x0261 = (_d_edge - -9.99999997E-07)/1.99999999E-06;
    _TMP43 = min(1.00000000E+00, _x0261);
    _TMP262 = max(0.00000000E+00, _TMP43);
    _edge_strength = _TMP262*_TMP262*(3.00000000E+00 - 2.00000000E+00*_TMP262);
    _r0269 = 5.00000000E-01*_TMP11.xyz;
    _r0269 = _r0269 + 5.00000000E-01*_TMP9.xyz;
    _r0271 = 5.00000000E-01*_TMP8.xyz;
    _r0271 = _r0271 + 5.00000000E-01*_TMP12.xyz;
    _color1 = _r0269 + _edge_strength*(_r0271 - _r0269);
    _TMP38 = min(_TMP11.xyz, _TMP12.xyz);
    _TMP39 = min(_TMP9.xyz, _TMP38);
    _TMP274 = min(_TMP8.xyz, _TMP39);
    _TMP20 = float((_d_edge >= 0.00000000E+00));
    _a0285 = (_TMP3.xyz - _TMP11.xyz)*(_TMP9.xyz - _TMP2.xyz);
    _b0285 = (_TMP1.xyz - _TMP8.xyz)*(_TMP12.xyz - _TMP4.xyz);
    _TMP21 = _a0285 + _TMP20*(_b0285 - _a0285);
    _min_sample = _TMP274 + _TMP21;
    _TMP40 = max(_TMP11.xyz, _TMP12.xyz);
    _TMP41 = max(_TMP9.xyz, _TMP40);
    _TMP286 = max(_TMP8.xyz, _TMP41);
    _TMP23 = float((_d_edge >= 0.00000000E+00));
    _a0297 = (_TMP3.xyz - _TMP11.xyz)*(_TMP9.xyz - _TMP2.xyz);
    _b0297 = (_TMP1.xyz - _TMP8.xyz)*(_TMP12.xyz - _TMP4.xyz);
    _TMP24 = _a0297 + _TMP23*(_b0297 - _a0297);
    _max_sample = _TMP286 - _TMP24;
    _TMP42 = min(_max_sample, _color1);
    _TMP298 = max(_min_sample, _TMP42);
    _ret_0 = vec4(_TMP298.x, _TMP298.y, _TMP298.z, 1.00000000E+00);
    FragColor = _ret_0;
    return;
} 
#endif
