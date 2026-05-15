#include <math.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 400
typedef struct{
 int indices[3];
}Triangle;

Vector3 vertices[8]={
        { 10.0f, -10.0f,  10.0f}, { 10.0f,  10.0f,  10.0f}, 
        { 10.0f, -10.0f, -10.0f}, { 10.0f,  10.0f, -10.0f},        
        {-10.0f, -10.0f,  10.0f}, {-10.0f,  10.0f,  10.0f}, 
        {-10.0f, -10.0f, -10.0f}, {-10.0f,  10.0f, -10.0f},
    };

    Triangle cubeT[12]={(Triangle){{2,1,0}},(Triangle){{1,2,3}},
                       (Triangle){{0,4,2}},(Triangle){{2,4,6}},
                       (Triangle){{6,3,2}},(Triangle){{3,6,7}},
                       (Triangle){{3,7,1}},(Triangle){{7,5,1}},
                       (Triangle){{7,6,5}},(Triangle){{4,5,6}},
                       (Triangle){{1,5,4}},(Triangle){{0,1,4}}};

Vector3 camera={0.0,0.0,100.0};

float theta=0.01;
/*
float rotations[3][3][3]={{{     1,        0,              0},
                              {     0,    cos(theta), -sin(theta)},
                              {     0,    sin(theta),  cos(theta)}},

                             {{cos(theta),      0,      -sin(theta)},
                              {     0,          1,         0       },
                              {sin(theta),      0,       cos(theta)}},

                             {{cos(theta), -sin(theta),   0},
                              {sin(theta),  cos(theta),   0},
                              {0,               0,        1}}};

*/
float rotations[3][3][3];


static void UpdateDrawFrame(void); 
void pointToDrawCube(Vector2 vertices[8]);
void raturize(Vector3 A,Vector3 B,Vector3 C,Color color);

    const float f=400;
    float Zindex[WIDTH][HEIGHT];

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

    rotations[0][0][0] = 1.0; rotations[0][0][1] = 0.0;         rotations[0][0][2] = 0.0;
    rotations[0][1][0] = 0.0; rotations[0][1][1] = cos(theta);  rotations[0][1][2] = -sin(theta);
    rotations[0][2][0] = 0.0; rotations[0][2][1] = sin(theta);  rotations[0][2][2] = cos(theta);

    // Y-Axis Matrix
    rotations[1][0][0] = cos(theta); rotations[1][0][1] = 0.0; rotations[1][0][2] = -sin(theta);
    rotations[1][1][0] = 0.0;        rotations[1][1][1] = 1.0; rotations[1][1][2] = 0.0;
    rotations[1][2][0] = sin(theta); rotations[1][2][1] = 0.0; rotations[1][2][2] = cos(theta);

    // Z-Axis Matrix
    rotations[2][0][0] = cos(theta); rotations[2][0][1] = -sin(theta); rotations[2][0][2] = 0.0;
    rotations[2][1][0] = sin(theta); rotations[2][1][1] = cos(theta);  rotations[2][1][2] = 0.0;
    rotations[2][2][0] = 0.0;        rotations[2][2][1] = 0.0;         rotations[2][2][2] = 1.0;

    InitWindow(800, 450, "raylib example - basic window");
    SetTargetFPS(60);
    while (!WindowShouldClose()){
        // z index maxxing 
    for(int x= 0;x<WIDTH;x++){
        for(int y=0;y<HEIGHT;y++)
            Zindex[x][y]=FLT_MAX;
    }

      UpdateDrawFrame();
    }

    CloseWindow();

    return 0;
}

Vector2 project(Vector3 point,Vector3 camera){
    if ((camera.z-point.z)<0.001f) {
        return (Vector2){ NAN, NAN };
    }
    Vector2 proj=(Vector2 ){(WIDTH/2.0)+((point.x-camera.x)*f/(camera.z-point.z)), (HEIGHT/2.0)+((point.y-camera.y)*f/(camera.z-point.z))};
    return proj;
}

// Update and draw game frame
static void UpdateDrawFrame(void){
    static int axis=0;
    static float rotation=0.0;
    //projection

    Vector2 projected[8];
    for(int i=0;i<8;i++){
        Rotation(rotations[axis],&(vertices[i]));
        projected[i] = project(vertices[i],camera);
    }

    BeginDrawing();
        ClearBackground(RAYWHITE);
        for(int i=0;i<12;i++){
            if( isnan(projected[(cubeT[i]).indices[0]].x) || isnan(projected[(cubeT[i]).indices[1]].x) || isnan(projected[(cubeT[i]).indices[2]].x)) continue;
            if( isnan(projected[(cubeT[i]).indices[0]].y) || isnan(projected[(cubeT[i]).indices[1]].y) || isnan(projected[(cubeT[i]).indices[2]].y)) continue;
            Color faceColor = (Color){(i*20)%255,(i*40)%255,(150+i*60)%225, 255 };
            raturize(vertices[(cubeT[i]).indices[0]], vertices[(cubeT[i]).indices[1]], vertices[(cubeT[i]).indices[2]], faceColor);
        }

    for (int i=0;i<8;i++){  
        DrawCircleV(projected[i], 5.0,(Color){ i*50,70+50*i,160+50*i, 225 } );
        char text[2]={(char)(i+'0'),'\0'};
        DrawText(text, projected[i].x, projected[i].y, 10, (Color){0, 0, 0, 225});
    }

    pointToDrawCube(projected);

    EndDrawing();
    //axis change
    rotation+=0.01;
    if(rotation>=(PI/4)){
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

void raturize(Vector3 A,Vector3 B,Vector3 C,Color color){
    
    Vector2 a=project(A,camera);
    Vector2 b=project(B,camera);
    Vector2 c=project(C,camera);
    if(isnan(a.x)||isnan(b.x)||isnan(c.x))  return;
    if(isnan(a.y)||isnan(b.y)||isnan(c.y))  return;

    float relAz=camera.z-A.z;
    float relBz=camera.z-B.z;
    float relCz=camera.z-C.z;

    int minX = (int)fminf(a.x, fminf(b.x, c.x));
    int minY = (int)fminf(a.y, fminf(b.y, c.y));
    int maxX = (int)fmaxf(a.x, fmaxf(b.x, c.x));
    int maxY = (int)fmaxf(a.y, fmaxf(b.y, c.y));

    for(int y=minY;y<=maxY;y++){
        for(int x=minX;x<=maxX;x++){

            float w0 = (b.x-a.x)*(y-a.y)-(b.y-a.y)*(x-a.x);
            float w1 = (c.x-b.x)*(y-b.y)-(c.y-b.y)*(x-b.x);
            float w2 = (a.x-c.x)*(y-c.y)-(a.y-c.y)*(x-c.x);
            //if(w0>=0&&w1>=0&&w2>=0 ||w0<=0&&w1<=0&&w2<=0){
            if(w0>=0&&w1>=0&&w2>=0){
                float totalArea=(b.x-a.x)*(c.y -a.y)-(b.y-a.y)*(c.x-a.x);

                if (fabsf(totalArea)<0.0001f)   return;

                float lambda1=w1/totalArea;
                float lambda2=w2/totalArea;
                float lambda0=1.0f-lambda1-lambda2;
                float pixelDepthToCam=(lambda1*relAz)+(lambda2*relBz)+(lambda0*relCz);
                if(pixelDepthToCam<Zindex[x][y]){
                    Zindex[x][y] = pixelDepthToCam;
                    DrawPixel(x,y,color);}
            }
        }
    }
  
}