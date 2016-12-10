package lab1;

//    class Triangle {
//        
//        float[] points = new float[3];
//        
//        public Triangle(float point1, float point2, float point3) {
//            points[0] = point1;
//            points[1] = point2;
//            points[2] = point3;
//        }
//        
//        public float getPointOne() {
//            return points[0];
//        }
//        
//        public float getPointTwo() {
//            return points[1];
//        }
//        
//        public float getPointThree() {
//            return points[2];
//        }
//        
//        public float intersects(float from, float dir) {
//            Ray ray = new Ray(from, dir);
//            return intersectRayTriangle(ray, this);
//        }
//        
//    }
//    
//    class Ray {
//        
//        float from = 0.0f;
//      float dir = 0.0f;
//        
//        public Ray(float from, float dir) {
//            this.from = from;
//            this.dir = dir;
//        }
//        
//        public float getStart() {
//            return from;
//        }
//        
//        public float getDirection() {
//            return dir;
//        }
//        
//    }
//    
//    
//    
//    public static final float SMALL_NUM = 0.00000001f;
//    
//    
//    public float intersectRayTriangle(Ray R, Triangle T) {
//        float I = new float();
//        float    u, v, n;
//        float    dir, w0, w;
//        float     r, a, b;
//        
//        u = new float(T.getPointTwo());
//        u.sub(T.getPointOne());
//        v = new float(T.getPointThree());
//        v.sub(T.getPointOne());
//        n = new float(); // cross product
//        n.cross(u, v);
//        
//        if (n.length() == 0) {
//            return null;
//        }
//        
//        dir = new float(R.getDirection());
//        w0 = new float(R.getStart());
//        w0.sub(T.getPointOne());
//        a = -(new float(n).dot(w0));
//        b = new float(n).dot(dir);
//        
//        if ((float)Math.abs(b) < SMALL_NUM) {
//            return null;
//        }
//        
//        r = a / b;
//        if (r < 0.0) {
//            return null;
//        }
//        
//        I = new float(R.getStart());
//        I.x += r * dir.x;
//        I.y += r * dir.y;
//        I.z += r * dir.z;
//        
//        return I;
//    }