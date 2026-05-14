#include <math.h>
#include <stdio.h>
#include <raylib.h>

static void UpdateDrawFrame(void); 
void pointToDrawCube(Vector2 vertices[8]);
    const int width=800;
    const int height=450;
    const float f=200;

void Rotation(float rotationalMatrix[3][3],Vector3 *point){
    float oldPoint[3]={point->x, point->y, point->z};
    float newPoint[3]={0.0,0.0,0.0};
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            newPoint[i]+=rotationalMatrix[i][j]*oldPoint[j];
        }
    }
    point->x=newPoint[0]; 
    point->y=newPoint[1]; 
    point->z=newPoint[2]; 
}

int main(void){
    InitWindow(800, 450, "raylib example - basic window");
    SetTargetFPS(60);
    while (!WindowShouldClose())    {
      UpdateDrawFrame();
    }

    CloseWindow();

    return 0;
}

Vector2 project(Vector3 point,Vector3 camera){
    Vector2 proj=(Vector2 ){400+(point.x*f/(camera.z-point.z)), 200+(point.y*f/(camera.z-point.z))};
    return proj;
}

// Update and draw game frame
static void UpdateDrawFrame(void){
    static float theta=0.0;
    Vector3 vertices[8]={
        { 10.0f, -10.0f,  10.0f}, { 10.0f,  10.0f,  10.0f}, 
        { 10.0f, -10.0f, -10.0f}, { 10.0f,  10.0f, -10.0f},        
        {-10.0f, -10.0f,  10.0f}, {-10.0f,  10.0f,  10.0f}, 
        {-10.0f, -10.0f, -10.0f}, {-10.0f,  10.0f, -10.0f},
    };
    Vector3 camera={0.0,0.0,50.0};

    
    // //rotation
    float rotaion[3][3]={{1,        0,              0},
                         {0,    cos(theta), -sin(theta)},
                         {0,    sin(theta),  cos(theta)}};
    for(int i=0;i<8;i++)
        Rotation(rotaion,&(vertices[i]));

    //projection
    Vector2 projected[8];
    for(int i=0;i<8;i++)
        projected[i] = project(vertices[i],camera);
    BeginDrawing();
        ClearBackground(RAYWHITE);
    for (int i=0;i<8;i++){
        //printf("(%f,%f)\n",projected[i].x,projected[i].y);
        DrawCircleV(projected[i], 5.0,(Color){ i*50,70+50*i,160+50*i, 225 } );
        pointToDrawCube(projected);
        char text[2]={(char)(i+'0'),'\0'};
        DrawText(text, projected[i].x, projected[i].y, 10, (Color){0, 0, 0, 225});
    }

    EndDrawing();
    theta+=0.01;
}

void pointToDrawCube(Vector2 vertices[8]){
DrawLineV(vertices[0],vertices[1], (Color){0, 0, 0, 225});
DrawLineV(vertices[0],vertices[2], (Color){0, 0, 0, 225});
DrawLineV(vertices[1],vertices[3], (Color){0, 0, 0, 225});
DrawLineV(vertices[2],vertices[3], (Color){0, 0, 0, 225});

DrawLineV(vertices[4],vertices[5], (Color){0, 0, 0, 225});
DrawLineV(vertices[4],vertices[6], (Color){0, 0, 0, 225});
DrawLineV(vertices[5],vertices[7], (Color){0, 0, 0, 225});
DrawLineV(vertices[6],vertices[7], (Color){0, 0, 0, 225});

DrawLineV(vertices[0],vertices[4], (Color){0, 0, 0, 225});
DrawLineV(vertices[1],vertices[5], (Color){0, 0, 0, 225});
DrawLineV(vertices[2],vertices[6], (Color){0, 0, 0, 225});
DrawLineV(vertices[3],vertices[7], (Color){0, 0, 0, 225});
}