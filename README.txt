building a brand new opengl game engine in c++ from scratch

opengl has its own quirks, obviously regarding optimization, memory management, legacy systems and so on
but i like it because of its simplicity
so i try to take advantage of modern opengl 4.6 features and mitigate almost many (if not all) of its problems

what articles i build upon:
https://www.tartley.com/files/2010/03/opengl-draw-calls.png
https://computergraphics.stackexchange.com/questions/37/what-is-the-cost-of-changing-state/46#46

what opengl features i plan to use (some of these mechanisms i already use, some i haven't taken advantage of):
- direct state access (so no more glBind)
- bufferstorage (so no more glSubData, change memory from the pointer directly)
- sparse bindless texture arrays (optimize texture bind calls)
- buffer fragment (rather that buffer per object, share ubo's for many objects) + persistent mapping
- for large amounts of data in shaders use SSBO (shader storage buffer object)
- ARB_multi_draw_indirect (to optimize draw calls validation when nothing changes (?))
- ARB_shader_draw_parameters (adds gl_DrawID to shaders, which command of multi_draw is executed)

what i already have:
- solid and robust opengl objects wrapper (rapier3d-gl)
it uses "Resource Aquisition is Initialization" c++ principle, bacisally turning opengl from state machine to object oriented system
- window framework based on glfw (rapier3d-framework)
- event-driven input system (InputProcessor.hpp)
- basic fly camera system (ViewCamera.hpp)
- .obj files support
.obj files are turned into GLobjmodel with render ready support 
and two demos (first is hot sexy girls image viewer, second is a 3d camera world with xyz colored axis and three models)

todo:
- gltf model, animation
- ecs
- physics, collisions
- embedded shaders (with uniforms defined by code, only real practical operations defined by shaders)
- framebuffer
- physically based rendering
- terrain generation
- imgui debugging