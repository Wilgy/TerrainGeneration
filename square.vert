///
// square.frag - A vertex shader that implements a basic
// Phong Illumination Model for 2D texture objects
//
// @author T. Wilgenbusch
///

// INCOMING DATA
// Homogeneous vertex coordinates
attribute vec4 vPosition;

// Normal vector at vertex (in model space)
attribute vec3 vNormal;

// Texture Coordinates at vertex
attribute vec2 vTexCoord; // Unused by the the 2D sampler

// Model transformations
uniform vec3 theta;
uniform vec3 trans;
uniform vec3 scale;

// Camera parameters
uniform vec3 cPosition;
uniform vec3 cLookAt;
uniform vec3 cUp;

// View volume boundaries
uniform float left;
uniform float right;
uniform float top;
uniform float bottom;
uniform float near;
uniform float far;

// Material properties
uniform vec4 ambColor;
uniform float ambRefCoef;

// Diffuse properties
// NOTE: The diffuse color in this case is the color gotten from the
// texture sampler
uniform vec4 difColor; // Unused
uniform float difRefCoef;

uniform vec4 specColor;
uniform float specExp;
uniform float specRefCoef;

// Light properties
uniform vec4 lightColor;
uniform vec4 lightPos;

// Ambient light color
uniform vec4 ambLightColor;

// OUTGOING DATA
// The base color of the object with only ambient color
varying vec4 color;
varying vec4 diffuse;

// The specular calculation and exponent
varying vec4 specular;
varying float exp;

// The tranformed normal and lighting vectors
varying vec4 normal;
varying vec4 lighting;

// The camera positions in view coords
varying vec4 viewCPos;

// The vertex position in model view coords
varying vec4 modelViewPos;

// To be interpolated by the fragment shader
varying vec2 texCoord;

void main()
{    
 // Compute the sines and cosines of each rotation about each axis
    vec3 angles = radians( theta );
    vec3 c = cos( angles );
    vec3 s = sin( angles );

    // Create rotation matrices
    mat4 rxMat = mat4( 1.0,  0.0,  0.0,  0.0,
                       0.0,  c.x,  s.x,  0.0,
                       0.0,  -s.x, c.x,  0.0,
                       0.0,  0.0,  0.0,  1.0 );

    mat4 ryMat = mat4( c.y,  0.0,  -s.y, 0.0,
                       0.0,  1.0,  0.0,  0.0,
                       s.y,  0.0,  c.y,  0.0,
                       0.0,  0.0,  0.0,  1.0 );

    mat4 rzMat = mat4( c.z,  s.z,  0.0,  0.0,
                       -s.z, c.z,  0.0,  0.0,
                       0.0,  0.0,  1.0,  0.0,
                       0.0,  0.0,  0.0,  1.0 );

    mat4 xlateMat = mat4( 1.0,     0.0,     0.0,     0.0,
                          0.0,     1.0,     0.0,     0.0,
                          0.0,     0.0,     1.0,     0.0,
                          trans.x, trans.y, trans.z, 1.0 );

    mat4 scaleMat = mat4( scale.x,  0.0,     0.0,     0.0,
                          0.0,      scale.y, 0.0,     0.0,
                          0.0,      0.0,     scale.z, 0.0,
                          0.0,      0.0,     0.0,     1.0 );

    // Create view matrix
    vec3 nVec = normalize( cPosition - cLookAt );
    vec3 uVec = normalize( cross (normalize(cUp), nVec) );
    vec3 vVec = normalize( cross (nVec, uVec) );

    mat4 viewMat = mat4( uVec.x, vVec.x, nVec.x, 0.0,
                         uVec.y, vVec.y, nVec.y, 0.0,
                         uVec.z, vVec.z, nVec.z, 0.0,
                         -1.0*(dot(uVec, cPosition)),
                         -1.0*(dot(vVec, cPosition)),
                         -1.0*(dot(nVec, cPosition)), 1.0 );

    // Create projection matrix
    mat4 projMat = mat4( (2.0*near)/(right-left), 0.0, 0.0, 0.0,
                         0.0, ((2.0*near)/(top-bottom)), 0.0, 0.0,
                         ((right+left)/(right-left)),
                         ((top+bottom)/(top-bottom)),
                         ((-1.0*(far+near)) / (far-near)), -1.0,
                         0.0, 0.0, ((-2.0*far*near)/(far-near)), 0.0 );

    // Transformation order:
    //    scale, rotate Z, rotate Y, rotate X, translate
    mat4 modelMat = xlateMat * rxMat * ryMat * rzMat * scaleMat;
    mat4 modelViewMat = viewMat * modelMat;

    // Transform the vertex location into clip space
    gl_Position =  projMat * viewMat  * modelMat * vPosition;

    // The vertex position and normal in model view coords
    vec4 MVP  = ( modelViewMat * vPosition );
    vec4 MVN  = ( modelViewMat * vec4( vNormal, 0.0) );

    // The light pos in view coords
    vec4 MVLP = ( viewMat * lightPos );

    // Normalize normal and light vectors
    vec4 N = ( normalize( MVN ) );
    vec4 L = ( normalize( MVLP - MVP ) );
    float dotLN = max( dot(L, N), 0.0);

    // Calculate the camera position vector in view coords
    vec4 VCP = viewMat * vec4( cPosition, 0.0);

    // Caculate the ambient light
    //       Light (Ia)    Material (Oa)   ka
    vec4 A = ambLightColor * ambRefCoef;

    // Calculate the diffuse light
    // Calculate by tex coord V
    //       Light (Id)  Material (Od)   kd       (L.N) 
    vec4 D = lightColor * difRefCoef * dotLN; 

    // Calculate the specular light
    //       Light (Id)  Material (Os)   ks        (R.V)^n passed to frag)
    vec4 S = lightColor * specColor * specRefCoef;

    // Pass all the necassary information to the fragment shader for the 
    // PHONG illumination model
    color = A;
    diffuse = D;
    specular = S;
    exp = specExp;

    normal = N;
    lighting = L;
    viewCPos = VCP;
    modelViewPos = MVP;

    // Pass on texture coords; since this is a cube map, we just 
    // use the original position of the vertex for the sampler
    texCoord = vPosition.xy;
}

