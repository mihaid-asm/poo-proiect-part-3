#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<memory>
#include<cmath>
using namespace std;

template<typename T>
class Point{
    char Name;
    T x,y,z;
public:
    Point() : Name('O'), x(0), y(0), z(0) {}
    Point(char N, T X, T Y, T Z) : Name(N), x(X), y(Y), z(Z) {}
    char getName() const {return Name;}
    T getX() const {return x;}
    T getY() const {return y;}
    T getZ() const {return z;}
    friend istream &operator>>(istream &in, Point<T> &P){
        in>>P.Name>>P.x>>P.y>>P.z;
        return in;
    }
    friend ostream &operator<<(ostream &out, const Point<T> &P){
        out<<P.Name<<"("<<P.x<<","<<P.y<<","<<P.z<<") ";
        return out;
    }
};

template<typename A>
A distance(const Point<A>& p1, const Point<A>& p2){
    A dx=p1.getX()-p2.getX();
    A dy=p1.getY()-p2.getY();
    A dz=p1.getZ()-p2.getZ();
    return sqrt(dx*dx+dy*dy+dz*dz);
}

class Shape{
public:
    virtual float Volume() const = 0;
    virtual ~Shape() {}
};

template<typename T>
class Sphere : public Point<T>, public Shape{
protected:
    T radius;
public:
    Sphere() : radius(0) {}
    Sphere(char N, T X, T Y, T Z, T R) : Point<T>(N, X, Y, Z), radius(R) {}
    Sphere(Point<T> P, T R) : Point<T>(P), radius(R) {}
    T AreaCircle() const {
        return M_PI*this->radius*this->radius;
    }
    T Volume() const override {
        return 4*M_PI*this->radius*this->radius*this->radius/3;
    }
    friend istream &operator>>(istream &in, Sphere<T> &S){
        in>>static_cast<Point<T>&>(S)>>S.radius;
        return in;
    }
    friend ostream &operator<<(ostream &out, const Sphere<T> &S){
        out<<static_cast<Point<T>&>(S)<<"R="<<S.radius;
        return out;
    }
};

template<typename T>
class Polygon{
protected:
    int no_points;
    vector<Point<T>> points;
public:
    Polygon() : no_points(0) {}
    Polygon(int N, vector<Point<T>> P) : no_points(N), points(P) {}
    T Area() const{
        T area=0;
        for(int i=1;i<no_points-1;i++){
            T L1 = sqrt(pow(points[i].getX() - points[0].getX(), 2) + pow(points[i].getY() - points[0].getY(), 2) + pow(points[i].getZ() - points[0].getZ(), 2));
            T L2 = sqrt(pow(points[i+1].getX() - points[i].getX(), 2) + pow(points[i+1].getY() - points[i].getY(), 2) + pow(points[i+1].getZ() - points[i].getZ(), 2));
            T L3 = sqrt(pow(points[i+1].getX() - points[0].getX(), 2) + pow(points[i+1].getY() - points[0].getY(), 2) + pow(points[i+1].getZ() - points[0].getZ(), 2));
            T SP = (L1+L2+L3)/2;
            area += sqrt(SP*(SP-L1)*(SP-L2)*(SP-L3));
        }
        return area;
    }
    T Perimeter() const{
        T peri=0;
        for(int i=0;i<no_points;i++){
            peri += sqrt(pow(points[i].getX() - points[(i + 1) % no_points].getX(), 2) + pow(points[i].getY() - points[(i + 1) % no_points].getY(), 2) + pow(points[i].getZ() - points[(i + 1) % no_points].getZ(), 2));
        }
        return peri;
    }
    friend istream &operator>>(istream &in, Polygon<T> &P){
        in>>P.no_points;
        P.points.resize(P.no_points);
        for(int i=0;i<P.no_points;i++){
            in>>P.points[i];
            Point Z = P.points[i];
            if(std::find(P.points.begin(), P.points.end(), Z) != std::end(P.points)){
                P.no_points--;
                i--;
            }
        }
        return in;
    }
    friend ostream &operator<<(ostream &out, const Polygon<T> &P){
        out<<P.no_points<<"\n";
        for(int i=0;i<P.no_points;i++){
            out<<P.points[i]<<"\n";
        }
        return out;
    }
};

template<typename T>
class Prism : public Polygon<T>, public Shape{
    T height;
public:
    Prism() : Polygon<T>(), height(0) {}
    Prism(int N, vector<Point<T>> P, T h) : Polygon<T>(N, P), height(h) {}
    T Volume() const {
        return this->Area() * height;
    }
    T TotalArea() const {
        return 2 * this->Area() + this->Perimeter() * height;
    }
    friend istream &operator>>(istream &in, Prism<T> &P){
        in>>static_cast<Polygon<T>&>(P)>>P.height;
        return in;
    }
    friend ostream &operator<<(ostream &out, const Prism<T> &P){
        out<<static_cast<Polygon<T>&>(P)<<P.height;
        return out;
    }
};

int main(){
    map<string, Sphere<float>> Spheres;
    map<string, Prism<float>> Prisms;
    while(true){
        int option;
        cout<<"1. Create a sphere.\n";
        cout<<"2. Create a prism.\n";
        cout<<"3. Calculate the volume of a shape.\n";
        cout<<"4. Exit.\n";
        cout<<"Make your choice: ";
        cin>>option;
        while(option!=1&&option!=2&&option!=3&&option!=4){
            cout<<"Invalid option.\n";
            cin>>option;
        }
        if(option==1){
            string name;
            Sphere<float> S1;
            cout<<"Name: ";
            cin>>name;
            cout<<"Sphere: ";
            cin>>S1;
            Spheres[name]=S1;
        }
        else if(option==2){
            string name;
            Prism<float> P1;
            cout<<"Name: ";
            cin>>name;
            cout<<"Prism: ";
            cin>>P1;
            Prisms[name]=P1;
        }
        else if(option==3){
            string name;
            cout<<"What shape do you want? ";
            cin>>name;
            if(Spheres.find(name)!=Spheres.end()){
                cout<<name<<"'s volume is "<<Spheres[name].Volume()<<"\n";
            }
            else if(Prisms.find(name)!=Prisms.end()){
                cout<<name<<"'s volume is "<<Prisms[name].Volume()<<"\n";
            }
            else{
                cout<<"Shape doesn't exist!\n";
            }
        }
        else break;
    }
    return 0;
}
