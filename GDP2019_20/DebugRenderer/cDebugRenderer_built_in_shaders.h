#ifndef _cDebugRenderer_built_in_shapes_
#define _cDebugRenderer_built_in_shapes_

#include "cDebugRenderer.h"


// ***************************************************************
//    ___     _     _           _            _          
//   | _ \___(_)_ _| |_ ___  __| |_  __ _ __| |___ _ _  
//   |  _/ _ \ | ' \  _(_-< (_-< ' \/ _` / _` / -_) '_| 
//   |_| \___/_|_||_\__/__/ /__/_||_\__,_\__,_\___|_|   
//                                                      
// ***************************************************************
/*static*/
const std::string cDebugRenderer::DEFAULT_VERT_SHADER_SOURCE_POINTS = "\
        #version 450                                         \n \
		                                                     \n \
        uniform mat4 mModel;                                 \n \
        uniform mat4 mView;                                  \n \
        uniform mat4 mProjection;                            \n \
		                                                     \n \
        in vec4 vPosition;		/* w is point size */        \n \
        in vec4 vColour;                                     \n \
		                                                     \n \
        out vec4 fVertColour;                                \n \
		                                                     \n \
        void main()                                          \n \
        {                                                    \n \
            mat4 MVP = mProjection * mView * mModel;         \n \
		                                                     \n \
            gl_Position = MVP * vec4(vPosition.xyz, 1.0f);   \n \
		                                                     \n \
            fVertColour = vColour;                           \n \
		                                                     \n \
            gl_PointSize = vPosition.w;                      \n \
        }\n";

/*static*/
const std::string cDebugRenderer::DEFAULT_FRAG_SHADER_SOURCE_POINTS = "\
        #version 450                                \n \
                                                    \n \
        in vec4 fVertColour;                        \n \
                                                    \n \
        out vec4 outputColour;                      \n \
                                                    \n \
        void main()                                 \n \
        {                                           \n \
            outputColour.rgb = fVertColour.rgb;     \n \
            outputColour.a = fVertColour.a;         \n \
        }\n	";
// ***************************************************************



// ***************************************************************
//    _    _                  _            _          
//   | |  (_)_ _  ___ ___  __| |_  __ _ __| |___ _ _  
//   | |__| | ' \/ -_|_-< (_-< ' \/ _` / _` / -_) '_| 
//   |____|_|_||_\___/__/ /__/_||_\__,_\__,_\___|_|   
//                                                    
// ***************************************************************
/*static*/
const std::string cDebugRenderer::DEFAULT_VERT_SHADER_SOURCE_LINES = "\
        #version 420                                    \n \
		                                                \n \
        uniform mat4 mModel;                            \n \
        uniform mat4 mView;                             \n \
        uniform mat4 mProjection;                       \n \
		                                                \n \
        in vec4 vPosition;                              \n \
        in vec4 vColour;                                \n \
		                                                \n \
        out vec4 gVertColour;                           \n \
		                                                \n \
        void main()                                     \n \
        {                                               \n \
            gl_Position = vPosition;                    \n \
		                                                \n \
            gVertColour = vColour;                      \n \
        }\n";

/*static*/
const std::string cDebugRenderer::DEFAULT_GEOM_SHADER_SOURCE_LINES = "\
        #version 400                                     \n \
                                                         \n \
        layout(lines)               in;                  \n \
        layout(line_strip)          out;                 \n \
        layout(max_vertices = 2)    out;		         \n \
	                                                     \n \
	    in vec4 gVertColour[];                           \n \
        out vec4 fVertColour;                            \n \
                                                         \n \
        uniform mat4 mModel;                             \n \
        uniform mat4 mView;                              \n \
        uniform mat4 mProjection;                        \n \
                                                         \n \
        void main()                                      \n \
        {                                                \n \
           mat4 MVP = mProjection * mView * mModel;      \n \
                                                         \n \
           gl_Position = MVP * gl_in[0].gl_Position;     \n \
           fVertColour = gVertColour[0];                 \n \
           EmitVertex();                                 \n \
                                                         \n \
           gl_Position = MVP * gl_in[1].gl_Position;     \n \
           fVertColour = gVertColour[1];                 \n \
           EmitVertex();                                 \n \
                                                         \n \
           EndPrimitive();                               \n \
        } \n";

/*static*/
const std::string cDebugRenderer::DEFAULT_FRAG_SHADER_SOURCE_LINES = "\
        #version 420                                \n \
                                                    \n \
        in vec4 fVertColour;                        \n \
                                                    \n \
	    out vec4 outputColour;                      \n \
                                                    \n \
        void main()                                 \n \
        {                                           \n \
            outputColour.rgb = fVertColour.rgb;     \n \
            outputColour.a = fVertColour.a;         \n \
        }\n	";

// ***************************************************************



// ***************************************************************
//    _____    _                _             _            _          
//   |_   _| _(_)__ _ _ _  __ _| |___ ___  __| |_  __ _ __| |___ _ _  
//     | || '_| / _` | ' \/ _` | / -_|_-< (_-< ' \/ _` / _` / -_) '_| 
//     |_||_| |_\__,_|_||_\__, |_\___/__/ /__/_||_\__,_\__,_\___|_|   
//                        |___/                                       
// ***************************************************************
/*static*/
const std::string cDebugRenderer::DEFAULT_VERT_SHADER_SOURCE_TRIANGLES = "\
        #version 420                                    \n \
		                                                \n \
        uniform mat4 mModel;                            \n \
        uniform mat4 mView;                             \n \
        uniform mat4 mProjection;                       \n \
		                                                \n \
        in vec4 vPosition;                              \n \
        in vec4 vColour;                                \n \
		                                                \n \
        out vec4 gVertColour;                           \n \
		                                                \n \
        void main()                                     \n \
        {                                               \n \
            gl_Position = MVP * vPosition;              \n \
		                                                \n \
            gVertColour = vColour;                      \n \
        }\n";

/*static*/
const std::string cDebugRenderer::DEFAULT_GEOM_SHADER_SOURCE_TRIANGLES = "\
        #version 400                                     \n \
                                                         \n \
        layout(triangles)               in;              \n \
        layout(triangle_strip)          out;             \n \
        layout(max_vertices = 3)        out;             \n \
	                                                     \n \
	    in vec4 gVertColour[];                           \n \
        out vec4 fVertColour;                            \n \
                                                         \n \
        uniform mat4 mModel;                             \n \
        uniform mat4 mView;                              \n \
        uniform mat4 mProjection;                        \n \
                                                         \n \
        void main()                                      \n \
        {                                                \n \
           mat4 MVP = mProjection * mView * mModel;      \n \
                                                         \n \
           gl_Position = MVP * gl_in[0].gl_Position;     \n \
           fVertColour = gVertColour[0];                 \n \
           EmitVertex();                                 \n \
                                                         \n \
           gl_Position = MVP * gl_in[1].gl_Position;     \n \
           fVertColour = gVertColour[1];                 \n \
           EmitVertex();                                 \n \
                                                         \n \
           gl_Position = MVP * gl_in[2].gl_Position;     \n \
           fVertColour = gVertColour[2];                 \n \
           EmitVertex();                                 \n \
                                                         \n \
           EndPrimitive();                               \n \
        } \n";

/*static*/
const std::string cDebugRenderer::DEFAULT_FRAG_SHADER_SOURCE_TRIANGLES = "\
        #version 420                                \n \
                                                    \n \
        in vec4 fVertColour;                        \n \
                                                    \n \
	    out vec4 outputColour;                      \n \
                                                    \n \
        void main()                                 \n \
        {                                           \n \
            outputColour.rgb = fVertColour.rgb;     \n \
            outputColour.a = fVertColour.a;         \n \
        }\n	";

// ***************************************************************


// ***************************************************************
//    __  __        _         _            _          
//   |  \/  |___ __| |_    __| |_  __ _ __| |___ _ _  
//   | |\/| / -_|_-< ' \  (_-< ' \/ _` / _` / -_) '_| 
//   |_|  |_\___/__/_||_| /__/_||_\__,_\__,_\___|_|   
//                                                    
// ***************************************************************
// Default geometry shader is a pass through



#endif
