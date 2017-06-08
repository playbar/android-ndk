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
COMPAT_VARYING     vec2 _blur_dxdy;
COMPAT_VARYING     vec2 _tex_uv1;
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
    vec2 _tex_uv1;
    vec2 _blur_dxdy;
};
out_vertex _ret_0;
input_dummy _IN1;
vec4 _r0006;
COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_ATTRIBUTE vec4 TexCoord;
COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec4 TEX1;
 
uniform mat4 MVPMatrix;
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    out_vertex _OUT;
    vec2 _dxdy_scale;
    vec2 _dxdy;
    _r0006 = VertexCoord.x*MVPMatrix[0];
    _r0006 = _r0006 + VertexCoord.y*MVPMatrix[1];
    _r0006 = _r0006 + VertexCoord.z*MVPMatrix[2];
    _r0006 = _r0006 + VertexCoord.w*MVPMatrix[3];
    _dxdy_scale = InputSize/OutputSize;
    _dxdy = _dxdy_scale/TextureSize;
    _OUT._blur_dxdy = vec2(_dxdy.x, 0.00000000E+00);
    _ret_0._position1 = _r0006;
    _ret_0._tex_uv1 = TexCoord.xy;
    _ret_0._blur_dxdy = _OUT._blur_dxdy;
    gl_Position = _r0006;
    TEX0.xy = TexCoord.xy;
    TEX1.xy = _OUT._blur_dxdy;
    return;
    TEX0.xy = _ret_0._tex_uv1;
    TEX1.xy = _ret_0._blur_dxdy;
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
COMPAT_VARYING     vec2 _blur_dxdy;
COMPAT_VARYING     vec2 _tex_uv1;
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
    vec2 _tex_uv1;
    vec2 _blur_dxdy;
};
vec4 _ret_0;
vec3 _TMP1;
float _TMP6;
float _TMP5;
float _TMP4;
vec4 _TMP2;
vec4 _TMP7;
uniform sampler2D Texture;
vec3 _TMP13;
float _w010016;
float _w01_ratio0016;
float _TMP17;
vec2 _tex_coords0022;
vec2 _tex_coords0028;
vec4 _color0034;
COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec4 TEX1;
 
uniform int FrameDirection;
uniform int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
void main()
{
    _TMP17 = pow(2.71828198E+00, -1.27322698E+00);
    _w010016 = 5.00000000E-01 + _TMP17;
    _w01_ratio0016 = _TMP17/_w010016;
    _tex_coords0022 = TEX0.xy - _w01_ratio0016*TEX1.xy;
    _TMP7 = COMPAT_TEXTURE(Texture, _tex_coords0022);
    _TMP2 = _TMP7;
    _tex_coords0028 = TEX0.xy + _w01_ratio0016*TEX1.xy;
    _TMP7 = COMPAT_TEXTURE(Texture, _tex_coords0028);
    _TMP13 = 5.00000000E-01*(_TMP2.xyz + _TMP7.xyz);
    _color0034 = vec4(_TMP13.x, _TMP13.y, _TMP13.z, 1.00000000E+00);
    _TMP4 = pow(_color0034.x, 4.54545438E-01);
    _TMP5 = pow(_color0034.y, 4.54545438E-01);
    _TMP6 = pow(_color0034.z, 4.54545438E-01);
    _TMP1 = vec3(_TMP4, _TMP5, _TMP6);
    _ret_0 = vec4(_TMP1.x, _TMP1.y, _TMP1.z, _color0034.w);
    FragColor = _ret_0;
    return;
} 
#endif
