#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "novas.h"
 
     
     /* Flag set by `--verbose'. */
     static int verbose_flag;
     
     int
     main (argc, argv)
          int argc;
          char **argv;
         
     {
       int c;

   short int error = 0;
   int i, j,ntimes;

/*
   'deltat' is the difference in time scales, TT - UT1.

    The variable 'tjd' contains Julian dates at which the
    star positions will be evaluated.
*/

   double deltat = 0.0;
   double tjd;
   double ra, dec,dis,rar,decr,hour,zd,az,lat,lon,elev,duration;
   short int year,month,day;
   double calc_increment;
   body earth;
   body sun;
   site_info geo_loc;
   
   geo_loc.temperature=10.0;
   geo_loc.pressure=1010.0;
   geo_loc.height=0;


       while (1)
         {
           static struct option long_options[] =
             {
               /* These options set a flag. */
               {"verbose", no_argument,       &verbose_flag, 1},
               {"brief",   no_argument,       &verbose_flag, 0},
               /* These options don't set a flag.
                  We distinguish them by their indices. */
               {"lat",     required_argument, 0, 'a'},
               {"lon",     required_argument, 0, 'o'},
               {"date",    required_argument, 0, 'd'},
               {"duration",required_argument, 0, 't'},
               {"time",    required_argument, 0, 'h'},
               {0, 0, 0, 0}
             };
           /* `getopt_long' stores the option index here. */
           int option_index = 0;
     
           c = getopt_long (argc, argv, "abc:d:f:",
                            long_options, &option_index);
     
           /* Detect the end of the options. */
           if (c == -1)
             break;
     
           switch (c)
             {
             case 0:
               /* If this option set a flag, do nothing else now. */
               if (long_options[option_index].flag != 0)
                 break;
               fprintf (stderr,"option %s", long_options[option_index].name);
               if (optarg)
                 fprintf (stderr," with arg %s", optarg);
               fprintf (stderr,"\n");
               break;
     
             case 'a':
              if (verbose_flag) fprintf (stderr,"option -a with value '%s'\n",optarg);
	       sscanf(optarg,"%lf",&geo_loc.latitude);
	      if (verbose_flag) fprintf(stderr,"lat %f\n",geo_loc.latitude);
               break;
     
             case 'o':
               if (verbose_flag) fprintf (stderr,"option -o with value '%s'\n",optarg);
               sscanf(optarg,"%lf",&geo_loc.longitude);
	       if (verbose_flag) fprintf(stderr,"lon %f\n",geo_loc.longitude );
               break;
     
             case 'd':
               if (verbose_flag) fprintf (stderr,"option -d with value `%s'\n", optarg);
               sscanf(optarg,"%4hd-%2hd-%2hd",&year,&month,&day);
	       if (verbose_flag) fprintf(stderr,"year %4hd month %02hd day %02hd\n",
		       year,month,day);
               break;
     
             case 't':
               if (verbose_flag) fprintf (stderr,"option -t with value `%s'\n", optarg);
	       sscanf(optarg,"%lf",&duration);
	       if (verbose_flag) fprintf(stderr,"duration %lf\n",duration);
               break;
     
             case 'h':
               if (verbose_flag) fprintf (stderr,"option -h with value `%s'\n", optarg);
	       sscanf(optarg,"%lf",&hour);
	       if (verbose_flag) fprintf(stderr,"time %lf\n",hour);
               break;
     
             case '?':
               /* `getopt_long' already printed an error message. */
               break;
     
             default:
               abort ();
             }
         }
     
       /* Instead of reporting `--verbose'
          and `--brief' as they are encountered,
          we report the final status resulting from them. */
       if (verbose_flag)
         fprintf (stderr,"verbose flag is set\n");
     
       /* Print any remaining command line arguments (not options). */
       if (optind < argc)
         {
           fprintf (stderr,"non-option ARGV-elements: ");
           while (optind < argc)
             fprintf (stderr,"%s ", argv[optind++]);
           fprintf (stderr,"\n");
         }
     

/*
   Main function to check out many parts of NOVAS-C by calling
   function 'topo_star' with version 3 of function 'solarsystem'.

   For use with NOVAS-C 2.0
*/

   	

/*   
   scanf("%lf %lf %lf %lf %hd %hd %hd %lf",
	 &geo_loc.latitude,&geo_loc.longitude,&geo_loc.height,
	 &duration,&year,&month,&day,&hour);
*/  

   tjd=julian_date(year,month,day,hour);

   if (verbose_flag) 
      fprintf (stderr,"%6.2f %6.2f %6.2f %4hd %2hd %2hd %12.9f %12.9f\n",
	    geo_loc.latitude,geo_loc.longitude,geo_loc.height,
	    year,month,day,hour,tjd); 

/*
   The observer's terrestrial coordinates (latitude, longitude, height).
*/



/*
   A structure containing the body designation for Earth.
*/

 
/*
   Set up the structure containing the body designation for Earth.
*/

   if (error = set_body (0,3,"Earth", &earth))
   {
      printf ("Error %d from set_body.\n", error);
      exit (1);
   }

   if (error = set_body (0,10,"Sun",&sun))
   {
	printf("Error %d from set_body.\n",error);
	exit(1);
   }

/*
   Compute the topocentric places of the sun at the selected Julian dates.
*/
   calc_increment = 1./240.;
   ntimes=duration/calc_increment+1;

   for (i = 0; i < ntimes; i++)
   {
         if (error = topo_planet (tjd,&sun,&earth,deltat,&geo_loc,
            &ra,&dec,&dis))
            printf ("Error %d from topo_planet   Time %d\n",
               error, i);
          else
         {
	    equ2hor(tjd,deltat,0,0,&geo_loc,ra,dec,1,&zd,&az,
		   &rar,&decr);	    
	    cal_date(tjd,&year,&month,&day,&hour);
	    zd=90-zd;
            printf ("%4d-%02d-%02d %5.2f UTC %6.2f %6.2f\n", 
			    year,month,day,hour,az,zd);
         }
      
      tjd+=calc_increment;
   }

   return (0);
}
