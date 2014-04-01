cbuffer CameraBuffer
{
    float3 eyePos;
    float nearPlane;
    float farPlane;
    float viewportW;
    float viewportH;
	int raymarchId;
    matrix viewInverse;
	
};

cbuffer LightBuffer
{
    float3 lightPosition;
	float paddingfd;
    float4 lightColor;
    
};

cbuffer BackgroundBuffer
{
    float4 backgroundColor;
}

struct Sphere {
   float3 centre;
   float rad2;
   float4 color;
   float Kd, Ks, Kr, shininess;
};

//static float4 sphereColor_1 = float4(0.0, 1.0, 0.0, 1.0);
//float4 sphereColor_2;
//float4 sphereColor_3;
//float shininess;

//int fDepth;
#define PI 3.14159265359
#define INTERVALS 100
#define SQR(N) (N*N)
#define NOBJECTS 1
static Sphere object[NOBJECTS] = {
   //sphere 1
	{ float3(0.0, 0.0, 0.0), SQR(1.0), float4(0.0, 1.0, 0.0, 1.0), 1.0, 1.0, 1.0, 128.0 },
 //  //sphere 2
	//{ 2.0, -1.0, 0.0, SQR(0.5), sphereColor_1, 1.0, 1.0, 1.0, 128.0 },
 //  //sphere3
	//{ 0.0, -201.5, 0.0, SQR(200.0), sphereColor_1, 0.8, 0.3, 0.3, 128.0 }
};

struct Ray {
   float3 o;
   float3 d;
};

float4 Phong(float3 norm, float3 Ldir, float3 Vdir, float shininess, float4 diffuseColor,
             float4 specularColor)
{
   float NdotL = dot(norm, Ldir);
   float diff = saturate(NdotL);
   float3 rDir = reflect(Ldir, norm);
   float spec = pow(saturate(dot(Vdir, rDir)), shininess);
   return diff * diffuseColor + spec*specularColor;
}


float4 Shade(float3 hitPos, float3 normal, float3 viewDir, int hitobj, float lightIntensity)
{
	//return float4(0.0, 1.0, 0.0, 1.0);
	//float3 lightDir = normalize(lightPosition - hitPos);
		float3 lightDir = normalize(float3(1.0, 1.0, -1.0));
   float4 diff = object[hitobj].color * object[hitobj].Kd;
   float4 spec = object[hitobj].color * object[hitobj].Ks;
   
   return lightColor*lightIntensity*Phong(normal, lightDir, viewDir, object[hitobj].shininess,
                                          diff, spec);                                    
}


float SphereIntersect(Sphere s, Ray ray, out bool hit)
{
   float t;
   float3 v = s.centre - ray.o;
   float A = dot(v, ray.d);
   float B = dot(v, v) - SQR(A);
   float R = sqrt(s.rad2);
   if(B > SQR(R))
   {
      hit = false;
      t = farPlane;
   }
   else
   {
      float disc = sqrt(R*R-B);
      t = A - disc;
      if(t < 0.0)
      {
         hit = false;        
      }
      else
      {
         hit = true;
      }
   }
   return t;
}


float3 SphereNormal(Sphere s, float3 pos)
{
   return normalize(pos - s.centre);
}

#define MIN_XYZ -200.1
#define MAX_XYZ 200.1
#define BOXMIN (float3)MIN_XYZ
#define BOXMAX (float3)MAX_XYZ
// Fine entering and leaving position of ray. (timeIn, timeOut).
bool IntersectBox(in Ray ray, in float3 minimum, in float3 maximum,
                  out float timeIn, out float timeOut)
{
   const float3 BoxMinimum = (float3)MIN_XYZ;
   const float3 BoxMaximum = (float3)MAX_XYZ;

   float3 OMIN = (minimum - ray.o) / ray.d;
   float3 OMAX = (maximum - ray.o) / ray.d;
   float3 MAX = max(OMAX, OMIN);
   float3 MIN = min(OMAX, OMIN);
   timeOut = min(MAX.x, min(MAX.y, MAX.z));
   timeIn  = max(max(MIN.x, 0.0), max(MIN.y, MIN.z));
   return timeOut > timeIn;
}

float softMax(float x, float y, float a)
{
   return log(exp(a*x) + exp(a*y))/a;
}

float smin(float a, float b, float k)
{
	float h = clamp(0.5 + 0.5*(b - a) / k, 0.0, 1.0);
	return lerp(b, a, h) - k*h*(1.0 - h);
}

float sdSphere(float3 p, float3 c, float r)
{
   return length(p - c) - r;
}

// P, position checking against, B box dimensions, R radius of edges
float udRoundBox(float3 p, float3 b, float r)
{
   return length(max(abs(p)-b, 0.0))-r;
}

float sdTorus(float3 p, float2 t)
{
   float2 q = float2(length(p.xz) - t.x, p.y);
   return length(q)-t.y;
}


float instanceTorus(float3 p, float3 c)
{
    float3 q = fmod(p, c) - 0.5*c;
    return sdTorus(q, float2(10, 2.0));
}

float sdCapsule(float3 p, float3 a, float3 b, float r)
{
   float3 pa = p - a;
   float3 ba = b - a;
   float h = clamp(dot(pa, ba)/dot(ba, ba), 0.0, 1.0);
   return length(pa - ba*h) - r;
}

float impInstanceSphere(float3 p, float3 c)
{
   float3 q = fmod(p, c) - 0.5 * c;
   return sdSphere(q, (float3)0, 5.0);
}


float blendedSpheres(float3 p, float3 c1, float3 c2, float r1, float r2, float smoothAmount)
{
	float sphere1 = sdSphere(p, c1, r1);
	float sphere2 = sdSphere(p, c2, r2);
	return smin(sphere1, sphere2, smoothAmount);
}


float penis(float3 Position)
{
   float fun1 = sdSphere(Position, float3(7.0, 0.0, 0.0), 6.0);
   float fun2 = sdSphere(Position, (float3)0, 6.0);
   float balls = smin(fun1, fun2, 1.5);
   float shaft = sdCapsule(Position, float3(3.5, 30.0, 0.0), float3(3.5, 0.0, 0.0), 5);
   float tip = sdSphere(Position, float3(3.5, 30.0, 0.0), 5.5);
   
   float result = smin(balls, shaft, 1.0);
   result = smin(result, tip, 2.0);
   return result;
}

float impInstancePenis(float3 p, float3 c)
{
   float3 q = fmod(p, c) - 0.5 * c;
   return penis(q);
}

float intersectionUnionTest(float3 p)
{
    float3 capsule1 = sdCapsule(p, float3(0.0, -25.0, 0.0), float3(0.0, +25.0, 0.0), 10.0);
    float3 capsule2 = sdCapsule(p, float3(-25.0, 0.0, 0.0), float3(+25.0, 0.0, 0.0), 10.0);
    float3 capsule3 = sdCapsule(p, float3(0.0, 0.0, -25.0), float3(0.0, 0.0, +25.0), 10.0);
    //float3 sphere1 = sdSphere(p, float3(0.0, 0.0, 0.0), 20.0);
    //return sphere1;
    float caps = min(capsule3, min(capsule1, capsule2));
    //return cap1cap2;
    //return max(-caps, sphere1);

    //return max(capsule1, capsule2);
    return smin(capsule3, smin(capsule1, capsule2, 8.0), 8.0);
    //return min(-sphere1, min(capsule3, min(capsule1, capsule2)));
}

float Function(float3 Position)
{
   float X = Position.x - 100.0;
   float Y = Position.y - 100.0;
   float Z = Position.z - 500.0;
   
   int test = raymarchId;
   switch(test)
   {
   case 0:
       return blendedSpheres(Position, float3(100.0, 100.0, 50.0), 
						float3(115.0, 100.0, 50.0), 
						8.0, 13.0, 4.0);
       break;
   case 1:
       float T = PI / 2.0;
       float Fun = 2.0 - cos(X + T * Y) - cos(X - T * Y) - cos(Y + T * Z) 
                  - cos(Y - T * Z) - cos(Z - T * X) - cos(Z + T * X);
       //float Fun = X * X + Y * Y - (1 - Z) * Z * Z;
       //float Fun = X * X * X + Y * Y * Y + Z * Z * Z - (X + Y + Z);
       return Fun;// - 1.0;
       break;
   case 2:
       return intersectionUnionTest(Position - float3(90.0, 100.0, 50.0));
       return impInstancePenis(Position, float3(50.0, 70.0, 50.0));
       break;
   case 3:
       return instanceTorus(Position - float3(30.0, 30.0, 50.0), float3(30.0, 20.0, 50.0));
       break;
   }
   
   
   return sdSphere(Position, float3(100.0, 100.0, 50.0), 10.0);
    

   return length(Position) - 5.0;
  
   //
}

bool IntersectSurface(in Ray ray, in float start, in float final, out float val)
{
   float step = (final - start) / float(INTERVALS);
   float time = start;
   float3 Position = ray.o + time * ray.d;
   float right, left = Function(Position);
   for(int i = 0; i < INTERVALS; ++i)
   {
      time += step;
      Position += step * ray.d;
      right = Function(Position);
      if(left * right < 0.0)
      {
         val = time + right * step / (left - right);
         return true;
      }
      left = right;
   }
   
   return false;
}


#define STEP 0.01
float3 CalcNormal(float3 Position)
{
	static const float3 Zero = float3(0.0, 0.0, 0.0);
	static const float3 Unit = float3(1.0, 1.0, 1.0);
	static const float3 AxisX = float3(1.0, 0.0, 0.0);
	static const float3 AxisY = float3(0.0, 1.0, 0.0);
	static const float3 AxisZ = float3(0.0, 0.0, 1.0);

    float A = Function(Position + AxisX * STEP)
                - Function(Position - AxisX * STEP);
    float B = Function(Position + AxisY * STEP)
                - Function(Position - AxisY * STEP);
    float C = Function(Position + AxisZ * STEP)
                - Function(Position - AxisZ * STEP);
    
    return normalize(float3(A, B, C));
}

float4 Raytrace(Ray ray)
{
   float4 result = (float4)0.0;
   float start, final;
   float t;

   if(IntersectBox(ray, BOXMIN, BOXMAX, start, final))
   {
      if(IntersectSurface(ray, start, final, t))
      {
         float3 Position = ray.o + ray.d * t;
         float3 normal = CalcNormal(Position);
         float3 color = (Position - BOXMIN) / (BOXMAX - BOXMIN);
         result = Shade(Position, normal, ray.d, 0, 1.0);
      }
   }
   
   return result;
}

float3 NearestHit(Ray ray, out int hitobj, out bool anyhit)
{
   float mint = farPlane;
   hitobj = -1;
   anyhit = false;
   for(int i = 0; i < NOBJECTS; i++)
   {
      bool hit;
      float t = SphereIntersect(object[i], ray, hit);
      if(hit)
      {
         if(t < mint)
         {
            hitobj = i;
            mint = t;
            anyhit = true;
         }
      }
   }
   return ray.o + ray.d*mint;
}

bool AnyHit(Ray ray)
{
   bool anyhit = false;
   for(int i = 0; i < NOBJECTS; i++)
   {
      bool hit;
      float t = SphereIntersect(object[i], ray, hit);
      if(hit)
      {
         anyhit = true;      
      }
   }
   return anyhit;
}

struct PixelInput 
{
   float4 Position : SV_POSITION;
   float2 Tex      : TEXCOORD0; 
};

float4 main(PixelInput input) : SV_Target
{   
   Ray eyeray;
   eyeray.o = float3(100.0, 100.00, -10.0);//eyePos.xyz;
   float3 dir;
   // scale the image shape such that it has the same aspect ratio as the viewport
   dir.xy = input.Tex * float2(1.0, viewportH/viewportW);
   dir.z = nearPlane;
   
   // Eye ray direction d is specified in view space. It must be transformed back into
   // world space.
   eyeray.d = mul(float4(dir, 0.0), viewInverse).xyz;
   eyeray.d = normalize(eyeray.d);
   
   return Raytrace(eyeray);
   // find the nearest hit
   //int hitobj;
   //bool hit;
   //float3 norm;
   //float4 RTColor = 0;
   //float lightIntensity = 1.0;
   //
   //// specify the eyeray
   //Ray currRay = eyeray;
   //
   //float3 IntSectP = NearestHit(eyeray, hitobj, hit);
   //
   //for(int depth =1; depth < fDepth; depth++)
   //{
   //   float depthIntensity = 1.0 / depth;
   //   if(hit)
   //   {
   //      Ray shadowRay; 
   //      shadowRay.d = normalize(lightPosition - IntSectP);
   //      shadowRay.o = IntSectP;
   //      norm = SphereNormal(object[hitobj], IntSectP);
   //      if(!AnyHit(shadowRay))
   //      {
   //         RTColor += depthIntensity * Shade(IntSectP, norm, currRay.d, hitobj, lightIntensity);
   //      }

   //      // shoot reflected ray
   //      lightIntensity *= object[hitobj].Kr;
   //      currRay.o = IntSectP;
   //      currRay.d = reflect(currRay.d, norm);
   //      IntSectP = NearestHit(currRay, hitobj, hit);
   //      
   //   }
   //   else
   //   {
   //      RTColor += backgroundColor;
   //   }
   //}
   //
   //return RTColor;
}



