#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

const int semicircle = 3.0; const int number = 2 * semicircle; const double a = 1; const double b = 1;
const double k = 1.e-4; const double epsilon = 1.e-6;
const double pi = acos(-1.0);

class Points{
private:
    double points[number][2];
    double forces[number][2];  
    double normal[number][2]; 
    double forces_tangential[number][2];

public:

    Points(double arr_rnd[number][2]){
        for (int i=0; i<number; i++){
            points[i][0] = arr_rnd[i][0];
            points[i][1] = arr_rnd[i][1];
            forces[i][0] = 0; forces[i][1] = 0;
            normal[i][0 ]= 0; normal[i][1] = 0;
            forces_tangential[i][0] = 0; forces_tangential[i][1] = 0;
        }    
    }

    ~Points(){}

     void count_forces () {
        for(int k=0; k<number; k++){
            double force_x = 0; double force_y = 0;
            for(int j=0; j<number; j++){
                if (points[j][0] != points[k][0]){
                    force_x -= 0.5 * (points[j][0] - points[k][0]) / pow(pow(points[j][0] - points[k][0], 2) + pow(points[j][1] - points[k][1], 2), 1.5);
				    force_y -= 0.5 * (points[j][1] - points[k][1]) / pow(pow(points[j][0] - points[k][0], 2) + pow(points[j][1] - points[k][1],2), 1.5);
                }
            }
            forces[k][0] = force_x; 
            forces[k][1] = force_y;
        }
    }

    double potential_energy(){
        double energy = 0;
        for(int k=0; k<number; k++)
            for(int j=0; j<number; j++)
                if (points[k][0] != points[j][0])
                    energy += 1 / pow(pow(points[j][0] - points[k][0], 2) + pow(points[j][1] - points[k][1], 2), 0.5);
        return energy;
    }

    void count_normal () {
        for(int j=0; j<number; j++){
           normal[j][0] =  (points[j][0] / pow(a, 2)) / pow(pow(points[j][0] / pow(a, 2), 2) + pow(points[j][1] / pow(b, 2), 2), 0.5);
           normal[j][1] =  (points[j][1] / pow(b, 2)) / pow(pow(points[j][0] / pow(a, 2), 2) + pow(points[j][1] / pow(b, 2), 2), 0.5);
        }    
    }

    void count_forces_tangential () {
        double force_n[2];
        for(int i=0; i<number; i++){
            force_n[0] = (forces[i][0] * normal[i][0] + forces[i][1] * normal[i][1]) * normal[i][0];
            force_n[1] = (forces[i][0] * normal[i][0] + forces[i][1] * normal[i][1]) * normal[i][1];
            forces_tangential[i][0] = forces[i][0] - force_n[0];
            forces_tangential[i][1] = forces[i][1] - force_n[1];
        }
    }

    void change_points () {
        for(int i=0; i<number; i++){
            points[i][0] += k * forces_tangential[i][0];
            points[i][1] += k * forces_tangential[i][1];
            double scaling_factor = pow(pow(points[i][0] / a, 2) + pow(points[i][1] / b, 2), -0.5);
            points[i][0] *= scaling_factor;
            points[i][1] *= scaling_factor;
        }
    }

    friend std::ostream& operator<< (std::ostream &out, const Points &points);

};

std::ostream& operator<< (std::ostream &out, const Points &points)
{
    for(int i=0; i<number; i++){
        out <<  points.points[i][0] << ", " << points.points[i][1] << endl;
    }
    return out; 
}

int main(){
    double arr_rnd[number][2];
    
    srand (static_cast <unsigned> (time(0)));
    
	for (int i=0; i<semicircle; i++){
        double alpha = 2 * pi * static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
        double x = a*b*tan(alpha)/pow(a*a + b*b*tan(alpha)*tan(alpha), 0.5);
        double y = x / tan(alpha);
        arr_rnd[i][0] = x; arr_rnd[i][1] = y;
    } 

    for (int i=semicircle; i<number; i++){
        double alpha = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
        double x = a*b*tan(alpha)/pow(a*a + b*b*tan(alpha)*tan(alpha), 0.5);
        double y = (-1) * x / tan(alpha);
        arr_rnd[i][0] = x; arr_rnd[i][1] = y;
    } 

    Points p = Points(arr_rnd);

    ofstream file;
    file.open("initial_points.txt");
    file << p;
    file.close();
 
    double energy_previous = p.potential_energy();
    double energy_current = 0;
    cout << energy_previous << ' ' << energy_current << endl;

    while(abs(energy_previous - energy_current) / energy_previous > epsilon){
        energy_previous = p.potential_energy();
        p.count_forces();
        p.count_normal();
        p.count_forces_tangential();
        p.change_points();
        energy_current = p.potential_energy();
        cout << 1;
    }

    cout << energy_previous << ' ' << energy_current << endl;
    
    ofstream file_result;
    file_result.open("result_points.txt");
    file_result << p;
    file_result.close();

    return 0;
}
