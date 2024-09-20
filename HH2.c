

#include<stdio.h>
#include<math.h>

float eq1(float x, float n, float m, float h, float Iext, float coup){ return (-1)*(36*n*n*n*n*(x-12) + 120*m*m*m*h*(x+115) + 0.3*(x+10.613)) - Iext - coup;}
float eq2(float x, float n, float m, float h){ return (0.01*(x+10))/(exp((x+10)/10)-1)*(1-n) - 0.125*exp(x/80)*n; }	
float eq3(float x, float n, float m, float h){ return (0.1*(x+25))/(exp((x+25)/10)-1)*(1-m) - 4*exp(x/18)*m; }
float eq4(float x, float n, float m, float h){ return 0.07*exp(x/20)*(1-h) - 1/(exp((x+30)/10)+1)*h; }

							

int main()
{    
    int N;
    int i = 0;															// time counter
    int j = 0; 															// neuron counter
    int k = 0;															// coupling counter
    int tc = 0, t;												// time step counter and amount of time steps
    float T, dt;													// time and time step
    float conn;
    
    FILE *params = fopen("parameters.dat", "r");
    fscanf(params, "%d%d%f%f", &N, &t, &dt, &conn);
    printf("N = %d\nt = %d\ndt = %f\nconn = %f\n", N, t, dt, conn);
    
    
    float w[N][N];									// coupling strength matrix
    FILE *coupling_matrix = fopen("coupling_matrix.dat", "r");
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {   
            fscanf(coupling_matrix, "%f", &w[i][j]);
            w[i][j] *= conn;
//             printf("%f\t", w[i][j]);
        }
//         printf("\n");
    }
    

//     w[N-1][0] = conn;
    
//     for (i = 0; i < N; i++)
//     {
//         for (j = 0; j < N; j++)
//         {
//             printf("%1.2f\t", w[i][j]);
//         }
//     printf("\n");
//     }
    
    float coup = 0;														// the sum by all couplings
	float Vsyn = 0;

    float x[N], x1[N], x2[N], x3[N], x4[N];					// dynamic variables and initial conditions
    float n[N], n1[N], n2[N], n3[N], n4[N];
    float m[N], m1[N], m2[N], m3[N], m4[N];
    float h[N], h1[N], h2[N], h3[N], h4[N];
    for (i = 0; i < N; i++)
    {
        x[i] = 10;
        n[i] = 0.1;
        m[i] = 0.1;
        h[i] = 0.1;
    }
    x[2] = 50;
    
    
    float Iext[N];											// external current
     for (i = 0; i < N; i++)
         Iext[i] = 3;
    Iext[75] = 4.2;
	tc = fabs(t/dt);  
    
	char filename[64];
	FILE *file;
	sprintf(filename, "HH_ens.dat", w[1][0], x[0]);
	file = fopen(filename, "w");
    
    //fprintf(file, "0.000000\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", x[0], n[0], m[0], h[0], 36*n[0]*n[0]*n[0]*n[0]*(x[0]-12) + 120*m[0]*m[0]*m[0]*h[0]*(x[0]+115) + 0.3*(x[0]+10.613), x[1], n[1], m[1], h[1], 36*n[1]*n[1]*n[1]*n[1]*(x[1]-12) + 120*m[1]*m[1]*m[1]*h[1]*(x[1]+115) + 0.3*(x[1]+10.613));
 		  
	for (i = 1; i < tc; i++){
	
			T = i*dt;
// 			fprintf(file, "%f\t", T);
						
		    for (j=0; j<N; j++){
				
				coup = 0;
				for (k = 0; k<N; k++) { coup += w[j][k]*(x[j]-x[k] - Vsyn); }
							
				x1[j] = eq1(x[j],n[j],m[j],h[j],Iext[j], coup);													
				n1[j] = eq2(x[j],n[j],m[j],h[j]);
				m1[j] = eq3(x[j],n[j],m[j],h[j]);
				h1[j] = eq4(x[j],n[j],m[j],h[j]);                   
				
           
				x2[j] = eq1(x[j]+dt*x1[j]/2,n[j]+dt*n1[j]/2,m[j]+dt*m1[j]/2,h[j]+dt*h1[j]/2,Iext[j], coup);		
				n2[j] = eq2(x[j]+dt*x1[j]/2,n[j]+dt*n1[j]/2,m[j]+dt*m1[j]/2,h[j]+dt*h1[j]/2);
				m2[j] = eq3(x[j]+dt*x1[j]/2,n[j]+dt*n1[j]/2,m[j]+dt*m1[j]/2,h[j]+dt*h1[j]/2);
				h2[j] = eq4(x[j]+dt*x1[j]/2,n[j]+dt*n1[j]/2,m[j]+dt*m1[j]/2,h[j]+dt*h1[j]/2);
            
				x3[j] = eq1(x[j]+dt*x2[j]/2,n[j]+dt*n2[j]/2,m[j]+dt*m2[j]/2,h[j]+dt*h2[j]/2,Iext[j], coup);		
				n3[j] = eq2(x[j]+dt*x2[j]/2,n[j]+dt*n2[j]/2,m[j]+dt*m2[j]/2,h[j]+dt*h2[j]/2);
				m3[j] = eq3(x[j]+dt*x2[j]/2,n[j]+dt*n2[j]/2,m[j]+dt*m2[j]/2,h[j]+dt*h2[j]/2);
				h3[j] = eq4(x[j]+dt*x2[j]/2,n[j]+dt*n2[j]/2,m[j]+dt*m2[j]/2,h[j]+dt*h2[j]/2);
            
				x4[j] = eq1(x[j]+dt*x3[j],n[j]+dt*n3[j],m[j]+dt*m3[j],h[j]+dt*h3[j],Iext[j], coup);				
				n4[j] = eq2(x[j]+dt*x3[j],n[j]+dt*n3[j],m[j]+dt*m3[j],h[j]+dt*h3[j]);
				m4[j] = eq3(x[j]+dt*x3[j],n[j]+dt*n3[j],m[j]+dt*m3[j],h[j]+dt*h3[j]);
				h4[j] = eq4(x[j]+dt*x3[j],n[j]+dt*n3[j],m[j]+dt*m3[j],h[j]+dt*h3[j]);
            
				x[j] = x[j] + dt/6*(x1[j] + 2*x2[j] + 2*x3[j] + x4[j]);
				n[j] = n[j] + dt/6*(n1[j] + 2*n2[j] + 2*n3[j] + n4[j]);
				m[j] = m[j] + dt/6*(m1[j] + 2*m2[j] + 2*m3[j] + m4[j]);
				h[j] = h[j] + dt/6*(h1[j] + 2*h2[j] + 2*h3[j] + h4[j]);
                if (i%10 == 0)                 
                    fprintf(file, "%f\t%d\t%f\n", T, j, x[j]);//, m[j], h[j], n[j], 36*n[j]*n[j]*n[j]*n[j]*(x[j]-12) + 120*m[j]*m[j]*m[j]*h[j]*(x[j]+115) + 0.3*(x[j]+10.613));
				}
   
            if (i%10 == 0) 
                fprintf(file, "\n");
          
        }
        
	fclose(file);
	//printf("The file is ready.\n");

}
