
/**************************************************************************
 * This file is part of TraceTuner, the DNA sequencing quality value,
 * base calling and trace processing software.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received (LICENSE.txt) a copy of the GNU General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *************************************************************************/

/* 
 * Copyright (c) 1999-2003 Paracel, Inc.  All rights reserved.
 *
 * 1.24 2003/11/06 18:18:42
 */

#include <math.h>
#include <float.h>
#include <stdio.h>

#include "Btk_qv.h"
#include "Btk_qv_data.h"
#include "Btk_qv_funs.h"  

#define EPS .0000001
#define EXP2_TABLE_SIZE 4000
#define EXP2_MAX_X      4.0

double tPhi[401] = {
0.425476, 0.436098, 0.453291, 0.464074, 0.478590, 0.488036, 0.501804, 0.509573,
0.520660, 0.529877, 0.537627, 0.548622, 0.555505, 0.562574, 0.571793, 0.579019,
0.585124, 0.592445, 0.601000, 0.606200, 0.612088, 0.619031, 0.626394, 0.631236,
0.636616, 0.642759, 0.650096, 0.654479, 0.659247, 0.664520, 0.670500, 0.676406,
0.680592, 0.685107, 0.690042, 0.695536, 0.701073, 0.704976, 0.709139, 0.713622,
0.718511, 0.723934, 0.727977, 0.731757, 0.735770, 0.740065, 0.744708, 0.749799,
0.753301, 0.756882, 0.760657, 0.764663, 0.768945, 0.773567, 0.777303, 0.780649,
0.784150, 0.787832, 0.791723, 0.795864, 0.800233, 0.803337, 0.806564, 0.809929,
0.813451, 0.817154, 0.821067, 0.825186, 0.828154, 0.831229, 0.834422, 0.837747,
0.841221, 0.844866, 0.848708, 0.851952, 0.854863, 0.857873, 0.860991, 0.864231,
0.867607, 0.871137, 0.874842, 0.877660, 0.880488, 0.883406, 0.886421, 0.889544,
0.892787, 0.896163, 0.899692, 0.902442, 0.905170, 0.907978, 0.910872, 0.913860,
0.916951, 0.920156, 0.923488, 0.926479, 0.929104, 0.931799, 0.934568, 0.937419,
0.940358, 0.943393, 0.946534, 0.949793, 0.952390, 0.954973, 0.957621, 0.960339,
0.963133, 0.966008, 0.968972, 0.972033, 0.975165, 0.977642, 0.980177, 0.982771,
0.985430, 0.988158, 0.990961, 0.993845, 0.996816, 0.999884, 1.002362, 1.004846,
1.007386, 1.009986, 1.012649, 1.015380, 1.018185, 1.021068, 1.024038, 1.026667,
1.029103, 1.031590, 1.034132, 1.036733, 1.039396, 1.042126, 1.044927, 1.047806,
1.050612, 1.053002, 1.055441, 1.057931, 1.060474, 1.063075, 1.065736, 1.068463,
1.071260, 1.074132, 1.076667, 1.079062, 1.081505, 1.083998, 1.086544, 1.089146,
1.091807, 1.094533, 1.097326, 1.100173, 1.102534, 1.104939, 1.107391, 1.109893,
1.112447, 1.115056, 1.117724, 1.120455, 1.123252, 1.125927, 1.128304, 1.130725,
1.133193, 1.135710, 1.138279, 1.140902, 1.143584, 1.146328, 1.149139, 1.151654,
1.154054, 1.156498, 1.158989, 1.161529, 1.164121, 1.166767, 1.169472, 1.172239,
1.175071, 1.177454, 1.179886, 1.182362, 1.184886, 1.187459, 1.190085, 1.192766,
1.195506, 1.198309, 1.200994, 1.203425, 1.205901, 1.208422, 1.210992, 1.213612,
1.216286, 1.219018, 1.221809, 1.224664, 1.227189, 1.229677, 1.232210, 1.234791,
1.237422, 1.240105, 1.242845, 1.245644, 1.248505, 1.251221, 1.253734, 1.256293,
1.258900, 1.261557, 1.264266, 1.267032, 1.269857, 1.272744, 1.275607, 1.278162,
1.280763, 1.283413, 1.286115, 1.288869, 1.291681, 1.294553, 1.297488, 1.300419,
1.303032, 1.305692, 1.308403, 1.311167, 1.313986, 1.316865, 1.319805, 1.322811,
1.325773, 1.328462, 1.331201, 1.333993, 1.336841, 1.339748, 1.342716, 1.345750,
1.348854, 1.351749, 1.354537, 1.357379, 1.360278, 1.363236, 1.366258, 1.369346,
1.372505, 1.375665, 1.378524, 1.381440, 1.384414, 1.387450, 1.390553, 1.393725,
1.396970, 1.400254, 1.403208, 1.406221, 1.409297, 1.412439, 1.415650, 1.418936,
1.422301, 1.425665, 1.428741, 1.431882, 1.435090, 1.438370, 1.441726, 1.445163,
1.448686, 1.452024, 1.455256, 1.458560, 1.461939, 1.465398, 1.468942, 1.472577,
1.476147, 1.479503, 1.482936, 1.486449, 1.490048, 1.493739, 1.497527, 1.501262,
1.504778, 1.508377, 1.512065, 1.515848, 1.519731, 1.523723, 1.527512, 1.531235,
1.535051, 1.538967, 1.542991, 1.547130, 1.551245, 1.555128, 1.559113, 1.563206,
1.567417, 1.571752, 1.576083, 1.580175, 1.584380, 1.588707, 1.593164, 1.597762,
1.602230, 1.606587, 1.611073, 1.615698, 1.620473, 1.625396, 1.629960, 1.634664,
1.639521, 1.644542, 1.649744, 1.654589, 1.659575, 1.664732, 1.670076, 1.675551,
1.680716, 1.686063, 1.691607, 1.697370, 1.703041, 1.708644, 1.714463, 1.720521,
1.726670, 1.732607, 1.738786, 1.745234, 1.751801, 1.758169, 1.764819, 1.771783,
1.778715, 1.785643, 1.792909, 1.800513, 1.807811, 1.815481, 1.823576, 1.831492,
1.839697, 1.848389, 1.856934, 1.865825, 1.875245, 1.884449, 1.894264, 1.904372,
1.914642, 1.925641, 1.936527, 1.948290, 1.960132, 1.972842, 1.985717, 1.999670,
2.013806, 2.029078, 2.045050, 2.062009, 2.080161, 2.099720, 2.120503, 2.143186,
2.168127, 2.195664, 2.226701, 2.261991, 2.303734, 2.354351, 2.419369, 2.511913,
2.682565};

double tW1[401] = {
0.832215, 0.832469, 0.833058, 0.833983, 0.835247, 0.836852, 0.838801, 0.841097,
0.843743, 0.846743, 0.850101, 0.853820, 0.857904, 0.862359, 0.867187, 0.872394,
0.877984, 0.883963, 0.890335, 0.897106, 0.904280, 0.911862, 0.919857, 0.928270,
0.937106, 0.946368, 0.956061, 0.966189, 0.976755, 0.987761, 0.999211, 1.011105,
1.023445, 1.036231, 1.049462, 1.063138, 1.077257, 1.091814, 1.106808, 1.122233,
1.138084, 1.154353, 1.171036, 1.188122, 1.205604, 1.223472, 1.241717, 1.260327,
1.279292, 1.298599, 1.318238, 1.338195, 1.358460, 1.379018, 1.399858, 1.420966,
1.442332, 1.463941, 1.485782, 1.507843, 1.530112, 1.552578, 1.575229, 1.598055,
1.621046, 1.644190, 1.667480, 1.690904, 1.714455, 1.738124, 1.761902, 1.785783,
1.809759, 1.833823, 1.857968, 1.882189, 1.906479, 1.930834, 1.955248, 1.979716,
2.004234, 2.028798, 2.053403, 2.078047, 2.102726, 2.127437, 2.152177, 2.176944,
2.201734, 2.226547, 2.251379, 2.276228, 2.301094, 2.325975, 2.350868, 2.375773,
2.400689, 2.425614, 2.450548, 2.475490, 2.500438, 2.525392, 2.550352, 2.575316,
2.600285, 2.625257, 2.650233, 2.675212, 2.700193, 2.725177, 2.750162, 2.775150,
2.800139, 2.825130, 2.850121, 2.875114, 2.900108, 2.925102, 2.950098, 2.975093,
3.000090, 3.025087, 3.050084, 3.075082, 3.100079, 3.125078, 3.150076, 3.175075,
3.200073, 3.225072, 3.250071, 3.275071, 3.300070, 3.325069, 3.350068, 3.375068,
3.400067, 3.425067, 3.450066, 3.475066, 3.500066, 3.525065, 3.550065, 3.575065,
3.600064, 3.625064, 3.650064, 3.675063, 3.700063, 3.725063, 3.750063, 3.775062,
3.800062, 3.825062, 3.850062, 3.875061, 3.900061, 3.925061, 3.950061, 3.975060,
4.000060, 4.025060, 4.050060, 4.075059, 4.100059, 4.125059, 4.150059, 4.175058,
4.200058, 4.225058, 4.250058, 4.275057, 4.300057, 4.325057, 4.350057, 4.375056,
4.400056, 4.425056, 4.450056, 4.475055, 4.500055, 4.525055, 4.550055, 4.575054,
4.600054, 4.625054, 4.650054, 4.675053, 4.700053, 4.725053, 4.750053, 4.775052,
4.800052, 4.825052, 4.850052, 4.875051, 4.900051, 4.925051, 4.950051, 4.975050,
5.000050, 5.025050, 5.050050, 5.075049, 5.100049, 5.125049, 5.150049, 5.175048,
5.200048, 5.225048, 5.250048, 5.275047, 5.300047, 5.325047, 5.350047, 5.375046,
5.400046, 5.425046, 5.450046, 5.475045, 5.500045, 5.525045, 5.550045, 5.575044,
5.600044, 5.625044, 5.650044, 5.675043, 5.700043, 5.725043, 5.750043, 5.775042,
5.800042, 5.825042, 5.850042, 5.875041, 5.900041, 5.925041, 5.950041, 5.975040,
6.000040, 6.025040, 6.050040, 6.075039, 6.100039, 6.125039, 6.150039, 6.175038,
6.200038, 6.225038, 6.250038, 6.275037, 6.300037, 6.325037, 6.350037, 6.375036,
6.400036, 6.425036, 6.450036, 6.475035, 6.500035, 6.525035, 6.550035, 6.575034,
6.600034, 6.625034, 6.650034, 6.675033, 6.700033, 6.725033, 6.750033, 6.775032,
6.800032, 6.825032, 6.850032, 6.875031, 6.900031, 6.925031, 6.950031, 6.975030,
7.000030, 7.025030, 7.050030, 7.075029, 7.100029, 7.125029, 7.150029, 7.175028,
7.200028, 7.225028, 7.250028, 7.275027, 7.300027, 7.325027, 7.350027, 7.375026,
7.400026, 7.425026, 7.450026, 7.475025, 7.500025, 7.525025, 7.550025, 7.575024,
7.600024, 7.625024, 7.650024, 7.675023, 7.700023, 7.725023, 7.750023, 7.775022,
7.800022, 7.825022, 7.850022, 7.875021, 7.900021, 7.925021, 7.950021, 7.975020,
8.000020, 8.025020, 8.050020, 8.075019, 8.100019, 8.125019, 8.150019, 8.175018,
8.200018, 8.225018, 8.250018, 8.275017, 8.300017, 8.325017, 8.350017, 8.375016,
8.400016, 8.425016, 8.450016, 8.475015, 8.500015, 8.525015, 8.550015, 8.575014,
8.600014, 8.625014, 8.650014, 8.675013, 8.700013, 8.725013, 8.750013, 8.775012,
8.800012, 8.825012, 8.850012, 8.875011, 8.900011, 8.925011, 8.950011, 8.975010,
9.000010, 9.025010, 9.050010, 9.075009, 9.100009, 9.125009, 9.150009, 9.175008,
9.200008, 9.225008, 9.250008, 9.275007, 9.300007, 9.325007, 9.350007, 9.375006,
9.400006, 9.425006, 9.450006, 9.475005, 9.500005, 9.525005, 9.550005, 9.575004,
9.600004, 9.625004, 9.650004, 9.675003, 9.700003, 9.725003, 9.750003, 9.775002,
9.800002, 9.825002, 9.850002, 9.875001, 9.900001, 9.925001, 9.950001, 9.975000,
10.000000};

double xminPhi=0.9404511322, dxPhi=0.0001486222, 
       xminW1=0.0001, dxW1=0.0249997500;

/*******************************************************************************
 * Function: Erf
 * Purpose: compute the error function
 *******************************************************************************
 */
double
Erf(double x)
{
    double p, a1, a2, a3;
    double t, sign;
 
    sign = (x==0)?0:(x/fabs(x));
    p  = 0.47047;
    a1 = 0.3480242;
    a2 =-0.0958798;
    a3 = 0.7478556;
    t = 1./(1.+p*x*sign);
 
    return sign*(1. - (a1*t + a2*t*t + a3*t*t*t) * exp(-x*x));
}


/*******************************************************************************
 * Function: exp2_table
 * Purpose: compute an approx. to exp(-x*x)
 *******************************************************************************
 */

static double 
exp2_table( double x )
{
    static double 
        factor = EXP2_TABLE_SIZE/EXP2_MAX_X,
        table[EXP2_TABLE_SIZE];
    static int initialized=0;
    int i;
    if( !initialized ) {
        double del = 1.0/factor;
        initialized = 1;
        for( i=0; i<EXP2_TABLE_SIZE; i++ ) {
            double x2 = (i+0.5)*del;
            x2 *= x2;
            table[i] = exp(-x2);
        }
    }
    if( x<0.0 ) { x = -x; }
    i = INT_DBL(factor*x);
    if( i>=EXP2_TABLE_SIZE ) return 0.0;
    return table[i];
}

/*******************************************************************************
 * Function: Shape
 * Purpose: compute the peak shape as a function of its model parameters
 *          and distance from the peak position
 *******************************************************************************
 */

static double
GaussianShape(double C0, double w0, double beta, double x)
{
    double X;
    if (beta <= EPS) {
        return 0.;
    }
    /* H = w0/beta; */ /* ignore w0 */
    X = x/beta;
    /*return C0 * exp(-X*X);*/
    return C0 * exp2_table(X);
}

static double
ConvolvedShape(double C0, double w0, double beta, double x)
{
   double H, X, C;

   if (beta <= EPS) {
       return 0.;
   }
 
   H = w0/beta;
   X = x/beta;
   C = 0.500001 * C0 * ( Erf(H-X) + Erf(H+X));
   if (C > EPS) {
       return C;
   }
   else {
      return 0;
   }
}

double 
Shape(double C0, double w0, double beta, double x, const Options *options)
{
    if (options->gauss) {
        return GaussianShape(C0, w0, beta, x);
    }

    return ConvolvedShape(C0, w0, beta, x);   

}

/*******************************************************************************
 * Function: Phi(x)
 * Purpose: compute the solution y(x) of: Erf(y)*W1(y)/y = x
 *******************************************************************************
 */

double Phi(double x)
{
    int    i;
   
    i = INT_DBL((x - xminPhi)/dxPhi); 

    if (i < 0) {
        return 0.425476;
    }
    else if (i >= 400) {
        return 2.682565;
    }
    else
    {
        return tPhi[i] + 
            (tPhi[i+1]-tPhi[i])/dxPhi * (x-xminPhi - dxPhi*(double)i);
    }
}

/*******************************************************************************
 * Function: W1(x)
 * Purpose: compute the solution x(y) of: Erf(y-x) + Erf(y+x) = Erf(y)
 *******************************************************************************
 */

double W1(double x)
{
    int    i;

    i = INT_DBL((x - xminW1)/dxW1);
 
    if (i < 0) {
        return 0.832215;
    }
    else if (i > 400) {
        return x;       
    }
    else
    {
        return tW1[i] + (tW1[i+1]-tW1[i])/dxW1 * (x-xminW1 - dxW1*(double)i);
    }
}