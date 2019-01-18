// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////
// includes
/////////////////////////////////////////////////////////
#include "mlRbf.h"
#include "math_fns.h"
#ifndef LINUX
#include "bar.h"
#include "utils.h"
#include "bench.h"
#endif
/////////////////////////////////////////////////////////
// shared globals
/////////////////////////////////////////////////////////


  __attribute__ ((section(".heapsram"))) static const float fv0[200] = { 15.290041F, 15.4762602F, 10.3755465F,
    13.5001993F, 14.0322876F, 4.61099052F, 4.39280462F, 2.67284942F, 5.94431829F,
    2.25745392F, 8.0767622F, 7.3019743F, 15.7322578F, 13.3488798F, 2.94789386F,
    5.68107939F, 13.6884928F, 12.8009539F, 16.9906158F, 19.6761055F,
    0.925916433F, 11.777997F, 0.368228614F, 10.6191654F, 0.194988832F,
    0.217154399F, 13.6329012F, 2.81823277F, 12.0630856F, 8.66298389F,
    7.52608824F, 0.297046334F, 5.41654158F, 0.138323724F, 0.77773571F,
    18.6210117F, 16.5453339F, 7.02357244F, 8.73151F, 19.5399303F, 6.60019064F,
    5.83220863F, 7.36680603F, 2.28136921F, 19.3918972F, 16.0225163F, 5.33213043F,
    16.3942909F, 16.1335945F, 5.87520885F, 6.02531385F, 5.24999475F, 19.7290897F,
    6.75892496F, 1.47904575F, 0.526839495F, 3.80391669F, 9.95940781F,
    1.90021729F, 8.95389652F, 10.1500015F, 17.1187572F, 10.7406368F, 18.6355076F,
    18.3452969F, 0.555999637F, 15.8685846F, 15.9729509F, 4.95943737F,
    0.779787302F, 9.83829212F, 17.1670132F, 6.82479191F, 9.86042F, 2.24966431F,
    3.5641582F, 14.3626928F, 12.1256113F, 15.2378426F, 19.1179142F, 11.2282629F,
    7.41185665F, 5.09285069F, 17.5762367F, 16.8148575F, 14.8398523F, 16.4529839F,
    13.869957F, 19.2090054F, 5.80809498F, 11.445797F, 5.05233431F, 17.6120205F,
    2.60245657F, 10.0080194F, 4.08659172F, 1.38503361F, 14.8480005F, 12.988987F,
    3.27706027F, 17.4900188F, 12.0531225F, 5.48911619F, 9.52900696F, 4.89822721F,
    0.225723907F, 17.0024319F, 16.943552F, 11.2590113F, 15.6249256F, 1.52548289F,
    0.595644474F, 17.8543262F, 4.29163742F, 12.6796646F, 6.5882597F, 12.239131F,
    18.3612671F, 6.2066493F, 1.21033323F, 9.69765568F, 16.6857452F, 14.0170593F,
    13.3357611F, 4.94342375F, 5.75539446F, 16.7729816F, 0.763784289F,
    2.45936227F, 16.109005F, 16.4139824F, 4.92699F, 3.9134047F, 15.9956093F,
    1.94623744F, 9.41162777F, 5.92939472F, 4.19844389F, 4.88834667F, 1.24645555F,
    14.7425756F, 1.68569279F, 18.4706497F, 14.0600452F, 14.4822102F, 17.8679142F,
    15.7262774F, 17.1517677F, 11.159811F, 4.15978622F, 14.1059647F, 17.3711395F,
    15.19765F, 11.9355545F, 19.380188F, 19.8768749F, 17.7900314F, 1.06868351F,
    9.12401485F, 5.46823168F, 3.71885967F, 1.37595475F, 9.73788F, 6.4015274F,
    4.85086918F, 16.70788F, 10.8253183F, 7.46087456F, 4.41260433F, 12.0460529F,
    13.5961294F, 1.62898898F, 18.7501411F, 13.490056F, 16.7138481F, 14.6072083F,
    2.11578393F, 18.5368347F, 15.6507626F, 9.7277832F, 16.355175F, 2.90471625F,
    14.7477055F, 19.4061508F, 8.63496113F, 7.2336359F, 10.1004848F, 10.0572023F,
    8.65605736F, 15.7475348F, 16.0754852F, 14.1031694F, 19.274231F, 7.06349325F,
    8.11272335F, 18.557272F, 12.9035959F, 13.991643F, 15.0213737F, 13.4319105F };

  __attribute__ ((section(".heapsram"))) static const float fv1[200] = { 5.98357105F, 0.949645F, 15.2962847F,
    13.044261F, 6.84219837F, 9.72431087F, 15.7595253F, 2.53547978F, 15.8627625F,
    10.3920536F, 2.32297063F, 10.1134434F, 18.5443058F, 4.60152674F, 7.00271606F,
    5.52242136F, 3.59842229F, 16.8141212F, 6.09284782F, 12.2323236F, 6.55858946F,
    14.9327784F, 14.3899202F, 0.496665835F, 0.148961127F, 8.30731487F,
    2.73158193F, 5.07270479F, 12.2572527F, 15.8980923F, 13.5001545F, 10.5105438F,
    1.91374266F, 10.615942F, 11.1104279F, 11.4299173F, 10.8179836F, 17.6568489F,
    16.9691315F, 9.78533649F, 4.8205533F, 5.5823946F, 1.39670789F, 7.09314823F,
    19.1643181F, 18.795723F, 17.2408218F, 6.49513102F, 3.10786939F, 4.4519949F,
    4.62676859F, 6.52972651F, 11.706789F, 15.2834377F, 18.9826813F, 3.00485682F,
    16.986002F, 7.58303547F, 3.8220284F, 1.56289184F, 10.6345348F, 7.28231859F,
    7.74861908F, 8.49266434F, 18.1812096F, 1.28499806F, 4.44129658F, 17.8726063F,
    10.6355047F, 6.18431091F, 15.6813364F, 0.159748495F, 17.8498096F,
    12.6260729F, 14.1962757F, 17.0141144F, 3.09490657F, 11.7138968F, 16.8698769F,
    0.883519351F, 2.28948927F, 3.67127132F, 2.40699673F, 10.4080858F,
    17.9235878F, 17.7729053F, 2.47567558F, 13.4194403F, 5.08785677F, 12.3243866F,
    9.12755585F, 17.4892941F, 9.51245403F, 15.2267103F, 7.98504925F, 3.20698309F,
    13.6588774F, 2.4109745F, 19.6468906F, 2.20081067F, 16.0007648F, 6.36625385F,
    8.96153164F, 16.7670784F, 0.0239006616F, 19.3618603F, 0.133938864F,
    15.2806797F, 9.87656116F, 11.3811131F, 19.7334824F, 10.305336F, 6.35351F,
    16.3336964F, 5.35770416F, 15.076705F, 8.59609127F, 5.29335546F, 2.9326489F,
    8.88874245F, 16.4194984F, 13.2713127F, 3.08307F, 11.5939646F, 7.71868086F,
    9.09458733F, 0.25638932F, 16.5721302F, 14.3369112F, 0.0476119891F,
    13.4650927F, 4.62163067F, 10.3102264F, 14.1630039F, 2.91793108F, 15.5763969F,
    10.2960835F, 10.2541323F, 19.8935795F, 6.81492376F, 14.2611465F, 9.49302F,
    3.5688448F, 4.70707846F, 18.4774647F, 17.286684F, 0.30142656F, 0.665863454F,
    19.0748653F, 14.7273083F, 4.98796701F, 12.2649364F, 6.06085491F, 10.1449175F,
    15.7400303F, 13.3220501F, 1.86924028F, 0.246622249F, 8.26998234F,
    12.7657385F, 12.8715277F, 17.823534F, 8.71684647F, 16.3745499F, 8.55739212F,
    6.78865767F, 5.88698292F, 12.2517185F, 13.4225636F, 15.0801334F, 12.9759893F,
    10.5166006F, 3.45290399F, 6.61558294F, 6.93462229F, 15.4020805F, 6.92850399F,
    1.20077109F, 14.4547596F, 7.54243279F, 6.60638857F, 7.97039652F, 4.5792551F,
    1.64797235F, 9.67592907F, 7.69380379F, 1.51448166F, 10.1852198F, 2.0268364F,
    4.40660906F, 19.3584709F, 4.0465F, 19.3376579F, 8.43509388F, 17.1420822F,
    2.05645204F, 17.8270779F, 14.0078459F, 11.126668F, 6.18572855F };

  __attribute__ ((section(".heapsram"))) static const float fv2[100] = { 0.409569174F, 0.0596155971F, 0.80968076F,
    0.684062541F, 0.0512121245F, 0.924186289F, 0.626638F, 0.337614805F,
    0.297795385F, 0.0931438133F, 0.276542962F, 0.563773215F, 0.115593798F,
    0.0149186049F, 0.879789054F, 0.241711274F, 0.872231483F, 0.286924332F,
    0.238982111F, 0.444692522F, 0.131513476F, 0.146381244F, 0.572100043F,
    0.223635092F, 0.781511605F, 0.491994739F, 0.252697229F, 0.309132904F,
    0.435964942F, 0.947770894F, 0.99437F, 0.685843468F, 0.0960800499F, 0.247838F,
    0.230108187F, 0.14145954F, 0.925802827F, 0.56918776F, 0.75007081F,
    0.64590013F, 0.858265698F, 0.163483918F, 0.34623906F, 0.758532524F,
    0.928790212F, 0.493177176F, 0.321140677F, 0.34930864F, 0.116693325F,
    0.415337831F, 0.538322747F, 0.211705133F, 0.364845514F, 0.287652135F,
    0.803365529F, 0.875197053F, 0.964248657F, 0.47025016F, 0.686368883F,
    0.797675371F, 0.706258893F, 0.773095965F, 0.0997264087F, 0.896923721F,
    0.0192555375F, 0.340630442F, 0.0926248133F, 0.678569376F, 0.339583933F,
    0.111013308F, 0.380679F, 0.244409144F, 0.66599685F, 0.494203687F,
    0.406710804F, 0.422271788F, 0.486513615F, 0.579239547F, 0.668070853F,
    0.0287827905F, 0.625473917F, 0.886429191F, 0.802671969F, 0.756492734F,
    0.292559087F, 0.398419142F, 0.33801043F, 0.465450108F, 0.00876136217F,
    0.88616854F, 0.628743827F, 0.988750398F, 0.766270578F, 0.862629056F,
    0.0888568535F, 0.457306713F, 0.346306175F, 0.437222868F, 0.266125172F,
    0.0214474462F };

  __attribute__ ((section(".heapsram"))) static const float fv3[80] = { 14.8532658F, 13.6970549F, 3.058424F,
    4.25366545F, 18.2001038F, 7.72203207F, 13.0513792F, 16.8631668F, 9.40613174F,
    10.6552734F, 18.2121277F, 14.3979673F, 8.53977299F, 5.05073786F, 0.421651F,
    7.32877731F, 3.27770257F, 2.76335812F, 5.53136539F, 9.05528736F, 1.94785428F,
    9.27947903F, 11.0236301F, 19.6575489F, 15.8587379F, 8.26637268F, 9.48909378F,
    15.2375164F, 1.99476504F, 2.35730934F, 0.665518582F, 14.602849F, 2.43096399F,
    3.70165634F, 16.6372108F, 0.252817631F, 18.4506721F, 12.9312458F, 11.41294F,
    12.151041F, 9.06314373F, 3.58670497F, 7.0027914F, 6.0992918F, 2.75835371F,
    17.972168F, 15.8139429F, 8.70573807F, 15.4309797F, 1.62482154F, 8.57296944F,
    16.3401203F, 17.8065071F, 9.80052567F, 13.7433243F, 5.23210335F, 19.6154308F,
    18.5268517F, 10.8255768F, 13.2153339F, 13.7303162F, 9.68154621F, 19.2106972F,
    8.05408382F, 3.84380603F, 4.38598585F, 10.5202579F, 14.7186985F, 19.2501926F,
    13.2179813F, 8.25203514F, 1.73099601F, 14.134059F, 5.84004F, 2.89453F,
    3.55199885F, 19.363121F, 1.65528929F, 2.69205976F, 9.22596836F };

  __attribute__ ((section(".heapsram"))) static const float fv4[16] = { 728.289856F, 728.27533F, 741.577148F,
    741.562378F, 905368.875F, 905350.75F, 398758.094F, 398750.094F,
    2.7807235E+6F, 2.780668E+6F, 1.74279712E+6F, 1.74276238E+6F, 827.81F,
    827.793396F, 759.577515F, 759.562378F };

/////////////////////////////////////////////////////////
// subfunctions
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// main testing function 
/////////////////////////////////////////////////////////
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  int coreid;
  int it;

  float x_hat[20];
  float y_hat[20];
  int nOut;
  float a[25];
  float w[25];
  float b_a;
  float b_w;
  float b_y;
  float z;
  float c_y;
  float b_y_hat[10];
  float c_y_hat[10];
  boolean_T pass, flag;
  float x[50];
  float y[50];
  float s[25];
  int i0;
  int b_k;


  init_fp_regs();

  /////////////////////////////////////////////////////////
  // main test loop 
  // each core loops over a kernel instance
  /////////////////////////////////////////////////////////

  coreid = get_core_id();

  printf("starting %d kernel iterations... (coreid = %d)\n",KERNEL_ITS,coreid);

  if (coreid>3)
    coreid=coreid-4;

  synch_barrier();

  perf_begin();

  for(it = 0; it < KERNEL_ITS; it++)
  {
    for (i0 = 0; i0 < 25; i0++) {
      for (b_k = 0; b_k < 2; b_k++) {
        x[b_k + (i0 << 1)] = fv0[(b_k + (i0 << 1)) + 50 * coreid];
        y[b_k + (i0 << 1)] = fv1[(b_k + (i0 << 1)) + 50 * coreid];
      }

      s[i0] = fv2[i0 + 25 * coreid];
    }

    for (i0 = 0; i0 < 10; i0++) {
      for (b_k = 0; b_k < 2; b_k++) {
        x_hat[b_k + (i0 << 1)] = fv3[(b_k + (i0 << 1)) + 20 * coreid];
      }
    }

    /*  mlRbf */
    //memset(&y_hat[0], 0.0F, 20U * sizeof(float));
    for (int idxk=0;idxk<20;idxk++)
      y_hat[idxk] = 0.0F;

    for (nOut = 0; nOut < 10; nOut++) {
      for (i0 = 0; i0 < 25; i0++) {
        b_a = x[i0 << 1] - x_hat[nOut << 1];
        b_w = b_a * b_a;
        b_a = x[1 + (i0 << 1)] - x_hat[1 + (nOut << 1)];
	b_w = -s[i0] * (b_w + b_a * b_a);
        b_w = fExp(b_w);
        a[i0] = y[i0 << 1] * b_w;
        w[i0] = b_w;
      }

      b_y = a[0];
      for (b_k = 0; b_k < 24; b_k++) {
        b_y += a[b_k + 1];
      }

      y_hat[nOut << 1] = b_y;
      for (i0 = 0; i0 < 25; i0++) {
        a[i0] = y[1 + (i0 << 1)] * w[i0];
      }

      b_y = a[0];
      for (b_k = 0; b_k < 24; b_k++) {
        b_y += a[b_k + 1];
      }

      y_hat[1 + (nOut << 1)] = b_y;
      b_y = w[0];
      for (b_k = 0; b_k < 24; b_k++) {
        b_y += w[b_k + 1];
      }

      z = fDiv(1.0F, b_y);
      for (i0 = 0; i0 < 20; i0++) {
        y_hat[i0] *= z;
      }
    }

  }

  synch_barrier();

  perf_end();

  /////////////////////////////////////////////////////////
  // check results
  /////////////////////////////////////////////////////////
  
  b_y = y_hat[0];
  c_y = y_hat[1];
  
  for (b_k = 0; b_k < 9; b_k++) {
    b_y += y_hat[(b_k + 1) << 1];
    c_y += y_hat[1 + ((b_k + 1) << 1)];
  }

  pass   = true;
  flag   = true;

  flag = flag && (b_y <= fv4[0 + (coreid << 2)]);
  flag = flag && (b_y >= fv4[1 + (coreid << 2)]);
  
  pass = pass && (c_y <= fv4[2 + (coreid << 2)]);
  pass = pass && (c_y >= fv4[3 + (coreid << 2)]);


  printErrors(!flag, 0, b_y, fv4[0 + (coreid << 2)],fv4[1 + (coreid << 2)]);
  printErrors(!pass, 1, c_y, fv4[2 + (coreid << 2)],fv4[3 + (coreid << 2)]);
  
  pass = flag && pass;
  
  flagPassFail(pass, coreid);
  
  synch_barrier();
/////////////////////////////////////////////////////////
// synchronize and exit
/////////////////////////////////////////////////////////

  return !pass;
}
