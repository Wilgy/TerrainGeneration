///
// square.frag - A fragment shader that implements a basic
// Phong Illumination Model for 2D textured objects
//
// @author T. Wilgenbusch
///

// The sampler for the current texture (may be unused)
uniform sampler2D texture;

// The base color of the object (before the specular highlight)
varying vec4 color;
varying vec4 diffuse;

// The specular calculation and exponent
varying vec4 specular;
varying float exp;

// The transformed normal and lighting vectors
varying vec4 normal;
varying vec4 lighting;

// The camera positions in view coords
varying vec4 viewCPos;

// The vertex position in model view coords
varying vec4 modelViewPos;

varying vec2 texCoord;

void main() 
{ 
    // Normalize given vectors
    vec4 N = normalize(normal);
    vec4 L = normalize(lighting);
    vec4 MVP = normalize(modelViewPos);
    vec4 VCP = normalize(viewCPos);

    // Compute the reflection and view vectors
    vec4 R = normalize( reflect(L, N)  );
    vec4 V = normalize( (MVP - VCP) );
    float dotRV = max(dot(R, V), 0.0);

    // The color of the texture
    vec4 texColor = texture2D(texture, texCoord);

    // Compute the final color with the specular highlight
    gl_FragColor = ( color + diffuse ) * texColor + ( specular * pow(dotRV, exp) );
} 
