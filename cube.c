#include <math.h>
#include <stdio.h>
#include <string.h>
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

void MatMult(float Mat1[3][3],float Mat2[3][3], float Mat3[3][3]){
    float Ans[3][3];
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            Ans[i][j]=0.0;
            for(int k=0;k<3;k++)
                Ans[i][j]+=Mat1[i][k]*Mat2[k][j];
        }
    }
    memcpy(Mat3, Ans, sizeof(Ans));
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
    static int axis=0;
    Vector3 vertices[8]={
        { 10.0f, -10.0f,  10.0f}, { 10.0f,  10.0f,  10.0f}, 
        { 10.0f, -10.0f, -10.0f}, { 10.0f,  10.0f, -10.0f},        
        {-10.0f, -10.0f,  10.0f}, {-10.0f,  10.0f,  10.0f}, 
        {-10.0f, -10.0f, -10.0f}, {-10.0f,  10.0f, -10.0f},
    };
    Vector3 camera={0.0,0.0,50.0};

    float theta=0.01;
    static float rotation=0.0;
float rotations[3][3][3]={{{     1,        0,              0},
                              {     0,    cos(theta), -sin(theta)},
                              {     0,    sin(theta),  cos(theta)}},

                             {{cos(theta),      0,      -sin(theta)},
                              {     0,          1,         0       },
                              {sin(theta),      0,       cos(theta)}},

                             {{cos(theta), -sin(theta),   0},
                              {sin(theta),  cos(theta),   0},
                              {0,               0,        1}}};


    static float super[3][3]={{1.0, 0.0, 0.0},
                              {0.0, 1.0, 0.0},
                              {0.0, 0.0, 1.0}};

     //rotation
    MatMult(rotations[axis],super,super);
   
    for(int i=0;i<8;i++)
        Rotation(super,&(vertices[i]));

    //projection
    Vector2 projected[8];
    for(int i=0;i<8;i++)
        projected[i] = project(vertices[i],camera);
    BeginDrawing();
        ClearBackground(RAYWHITE);
    for (int i=0;i<8;i++){  
        DrawCircleV(projected[i], 5.0,(Color){ i*50,70+50*i,160+50*i, 225 } );
        pointToDrawCube(projected);
        char text[2]={(char)(i+'0'),'\0'};
        DrawText(text, projected[i].x, projected[i].y, 10, (Color){0, 0, 0, 225});
    }

    EndDrawing(); //rotation
    rotation+=0.01;
    if(rotation>=PI/4){
        rotation=0;
        axis=(axis+1)%3;
    }
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