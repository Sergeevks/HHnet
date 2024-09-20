#include<stdio.h>
#include<math.h>

float eq1(float x, float n, float m, float h, int I){ return (-1)*(36*n*n*n*n*(x-12) + 120*m*m*m*h*(x+115) + 0.3*(x+10.613)) - I; }
float eq2(float x, float n, float m, float h){ return (0.01*(x+10))/(exp((x+10)/10)-1)*(1-n) - 0.125*exp(x/80)*n; }	
float eq3(float x, float n, float m, float h){ return (0.1*(x+25))/(exp((x+25)/10)-1)*(1-m) - 4*exp(x/18)*m; }
float eq4(float x, float n, float m, float h){ return 0.07*exp(x/20)*(1-h) - 1/(exp((x+30)/10)+1)*h; }	
							

int main()
{
    int i = 0;							// counter	
    int tc = 0, t = 1000;				// time step counter and amount of time steps
    float T, dt = 0.01;					// time and time step

    int I = 12;							// external current
     
    float x = 10, x1, x2, x3, x4;		// dynamic variables and initial conditions
    float n = 0.5, n1, n2, n3, n4, n_0;
    float m = 0.01, m1, m2, m3, m4, m_0;
    float h = 0.01, h1, h2, h3, h4, h_0;
    float min, max, aver;
    int osc = 0;

	tc = fabs(t/dt);

	//printf("Enter the initial conditions: x0 n0 m0 h0.\n");
    //scanf("%f %f %f %f", &x, &n, &m, &h);
    //printf("Please, wait.\n");  
    
	char filename[64];
	FILE *file;
	sprintf(filename, "HHsol_n=%f.dat", n);
	file = fopen(filename, "w");
    
    for (n_0 = 0; n_0 < 1; n_0 += 0.01)
        for (m_0 = 0; m_0 < 1; m_0 += 0.01)
            for (h_0 = 0; h_0 < 1; h_0 += 0.01)
            {
            n = n_0;
            m = m_0;
            h = h_0;
            x = 10;
                
            //fprintf(file, "0.000000\t%f\t%f\t%f\t%f\t0.000000\t0.000000\n", x, n, m, h);
            min = 100000;
            max = -100000;
            aver = 0;
                for (i = 0; i < tc; i++) 
                {
                
                    x1 = eq1(x,n,m,h,I);
                    n1 = eq2(x,n,m,h);
                    m1 = eq3(x,n,m,h);
                    h1 = eq4(x,n,m,h);
                    
                    x2 = eq1(x+dt*x1/2,n+dt*n1/2,m+dt*m1/2,h+dt*h1/2,I);
                    n2 = eq2(x+dt*x1/2,n+dt*n1/2,m+dt*m1/2,h+dt*h1/2);
                    m2 = eq3(x+dt*x1/2,n+dt*n1/2,m+dt*m1/2,h+dt*h1/2);
                    h2 = eq4(x+dt*x1/2,n+dt*n1/2,m+dt*m1/2,h+dt*h1/2);
                    
                    x3 = eq1(x+dt*x2/2,n+dt*n2/2,m+dt*m2/2,h+dt*h2/2,I);
                    n3 = eq2(x+dt*x2/2,n+dt*n2/2,m+dt*m2/2,h+dt*h2/2);
                    m3 = eq3(x+dt*x2/2,n+dt*n2/2,m+dt*m2/2,h+dt*h2/2);
                    h3 = eq4(x+dt*x2/2,n+dt*n2/2,m+dt*m2/2,h+dt*h2/2);
                    
                    x4 = eq1(x+dt*x3,n+dt*n3,m+dt*m3,h+dt*h3,I);
                    n4 = eq2(x+dt*x3,n+dt*n3,m+dt*m3,h+dt*h3);
                    m4 = eq3(x+dt*x3,n+dt*n3,m+dt*m3,h+dt*h3);
                    h4 = eq4(x+dt*x3,n+dt*n3,m+dt*m3,h+dt*h3);
                    
                    x = x + dt/6*(x1 + 2*x2 + 2*x3 + x4);
                    n = n + dt/6*(n1 + 2*n2 + 2*n3 + n4);
                    m = m + dt/6*(m1 + 2*m2 + 2*m3 + m4);
                    h = h + dt/6*(h1 + 2*h2 + 2*h3 + h4);
                    

                            
                    T = i*dt;
                    
                    if (i > tc*0.9)
                    {
                        aver += x;
                        if (x < min)
                            min = x;
                        if (x > max)
                            max = x;
                    }
                    
                    //fprintf(file, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", T, x, n, m, h, 36*n*n*n*n, 120*m*m*m*h, 36*n*n*n*n*(x-12) + 120*m*m*m*h*(x+115) + 0.3*(x+10.613));
                    
                }
                aver = aver /(0.1*tc);
                if (max - min <= fabs(0.1*aver))
                    osc = 0;
                else
                    osc = 1;
                printf("%f\t%f\t%f\t%f\t%f\t%f\t%d\n", n_0,m_0,h_0,min,max,aver, osc);
                fprintf(file, "%f\t%f\t%f\t%f\t%f\t%f\t%d\n", n_0,m_0,h_0,min,max,aver, osc);
            }
	fclose(file);
	//printf("The file is ready.\n");

}
