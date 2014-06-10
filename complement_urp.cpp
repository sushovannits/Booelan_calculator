#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef enum {dont_exist=-2,neg,dont_care,pos} values;
int num_times=0;

//num_terms is always plus one so browse from 1 to num_terms-1

//Function to print PCN
void print_pcn(int **cube) {
   int num_vars=cube[0][0];
   int num_terms=cube[0][1];
   printf("Printing cube\n");
   printf("**************\n");
   printf("num_vars: %d num_terms: %d\n",num_vars,num_terms);
   for(int row=1;row<num_terms;row++) {
      for(int col=0;col<num_vars;col++) {
         printf("%2d ",cube[row][col]);
      }
      printf("\n");
   }
}
//Function to print in submittable format
void print_pcn_output_format(int **cube){
   int num_vars=cube[0][0];
   int num_terms=cube[0][1];
   int actual_num_terms=0;int num_dont_exist_cubes=0;
   //Finding non existent cubes
   for(int term_row=1;term_row<num_terms;term_row++){
      if(cube[term_row][0]==dont_exist) num_dont_exist_cubes++; 
   }
   actual_num_terms=num_terms-num_dont_exist_cubes-1;
   printf("%d\n",num_vars);
   printf("%d\n",actual_num_terms);
   for(int term_row=1;term_row<num_terms;term_row++){
      if(cube[term_row][0]==dont_exist) continue;
      int num_vars_pos_neg=0;
      for(int var_col=0;var_col<num_vars;var_col++) 
         if(cube[term_row][var_col]==pos || cube[term_row][var_col]==neg) num_vars_pos_neg++;
      printf("%d",num_vars_pos_neg);
      for(int var_col=0;var_col<num_vars;var_col++){ 
         if(cube[term_row][var_col]==dont_care) continue;
         if(cube[term_row][var_col]==pos)
            printf(" %d",var_col+1);
         else if(cube[term_row][var_col]==neg)
            printf(" -%d",var_col+1);
      }
      printf("\n");   
   }
      
}

//Function to allocate memory for PCN
int **allocate_pcn(int num_vars,int num_terms) {
   int **cube=NULL;
   cube=(int **)calloc(num_terms+1,sizeof(int *));
   for(int row_index=0;row_index<(num_terms+1);row_index++){ 
      cube[row_index]=(int *)calloc(num_vars,sizeof(int));
   }
   cube[0][0]=num_vars;
   cube[0][1]=num_terms+1;
   return cube;
}
void deallocate_pcn(int **cube) {
   int num_vars=cube[0][0];
   int num_terms=cube[0][1];
   for(int row_index=1;row_index<num_terms;row_index++){
      free(cube[row_index]);cube[row_index]=NULL;
   }
   free(cube);cube=NULL;
}
//Function to copy
void copy_pcn(int **from_cube,int **to_cube){
   int num_vars=from_cube[0][0];
   int num_terms=from_cube[0][1];
   for(int term_row=0;term_row<num_terms;term_row++)
      memcpy(&to_cube[term_row][0],&from_cube[term_row][0],sizeof(int)*num_vars);
}
//Function to AND a vriable to a cube
void and_var_pcn(int **cube,int var,values pos_neg_value) {
   int num_vars=cube[0][0];
   int num_terms=cube[0][1];
   for(int term_row=1;term_row<num_terms;term_row++){
      if(cube[term_row][var]==0)cube[term_row][var]=pos_neg_value;
   }
}
//Function to find if all elements ina row are dont_exist
bool is_all_dont_exist_row(int *cube_row,int num_vars) {
   int num_dont_exist;
   for(int var_col=0;var_col<num_vars;var_col++)
      if(cube_row[var_col]==dont_exist) num_dont_exist++;
   if(num_dont_exist==num_vars) return true;
   else return false;
         
}
//Function to OR a vriable to a cube
int **or_pcn(int **cube1,int **cube2,int var_x) {
   int **final_cube=NULL;
   int num_vars_cube1=cube1[0][0];
   int num_terms_cube1=cube1[0][1];
   int num_vars_cube2=cube2[0][0];
   int num_terms_cube2=cube2[0][1];
   int num_vars_final_cube=num_vars_cube1;
   int num_terms_final_cube=num_terms_cube1+num_terms_cube2-2;//-1 is the size storage
   final_cube=allocate_pcn(num_vars_final_cube,num_terms_final_cube);
   num_vars_final_cube=final_cube[0][0];
   num_terms_final_cube=final_cube[0][1];

   for(int final_term_row=1;final_term_row<num_terms_final_cube;) {
      for(int term_row=1;term_row<num_terms_cube1;term_row++){
         memcpy(&final_cube[final_term_row][0],&cube1[term_row][0],sizeof(int)*num_vars_cube1);final_term_row++;
      }
      for(int term_row=1;term_row<num_terms_cube2;term_row++){
         memcpy(&final_cube[final_term_row][0],&cube2[term_row][0],sizeof(int)*num_vars_cube2);final_term_row++;
      }
   }
   return final_cube;
}
//Function to find most binate variable
int find_most_binate_variable(int **cube){
   int num_vars=cube[0][0];
   int num_terms=cube[0][1];
   int *score_for_pos=(int *)calloc(num_vars,sizeof(int));
   int *score_for_neg=(int *)calloc(num_vars,sizeof(int));;//T-C
   //Appearance score check
   for(int term_row=1;term_row<num_terms;term_row++) {
      for(int var_col=0;var_col<num_vars;var_col++) {
         if(cube[term_row][var_col]==pos)
            score_for_pos[var_col]++;
         if(cube[term_row][var_col]==neg)
            score_for_neg[var_col]++;
      }
   }

   int binate_var_index=0;int most_pos_or_neg=0;int least_diff=num_vars;
   for(int var_col=0;var_col<num_vars;var_col++) {
      printf("var %d score_for_pos score_for_pos %d score_for_neg %d\n",var_col,score_for_pos[var_col],score_for_neg[var_col]);
      int curr_diff=abs(score_for_pos[var_col]- score_for_neg[var_col]);
      if((score_for_pos[var_col]+score_for_neg[var_col])>most_pos_or_neg){
         binate_var_index=var_col;
         least_diff=curr_diff;
         most_pos_or_neg=score_for_pos[var_col]+score_for_neg[var_col];
      }else if((score_for_pos[var_col]+score_for_neg[var_col])==most_pos_or_neg){
         if(curr_diff<least_diff){
            binate_var_index=var_col;
            least_diff=curr_diff;
            most_pos_or_neg=score_for_pos[var_col]+score_for_neg[var_col];
         }
      }
            
      
   }
   printf("Most binate variable is %d\n",binate_var_index);
   free(score_for_pos);score_for_pos=NULL;
   free(score_for_neg);score_for_neg=NULL;
   return binate_var_index;
}

//Function to find most binate variable


//Function to find cofactor
//We will delete the original PCN and form two new PCN in every step. So at leaf nodes of recursion tree there will be only few cube lists.
int **find_cofactor(int **input_cube,int var_x,bool pos_cofactor){
   int num_vars=input_cube[0][0];
   int num_terms=input_cube[0][1];
   int **output_cube=NULL;
   output_cube=allocate_pcn(num_vars,num_terms-1);
   //var_x is the variable around which to cofactor and it also serves as the index
   //printf("After copy\n");
   copy_pcn(input_cube,output_cube);
   //print_pcn(output_cube,num_vars,num_terms);
   for(int term_row=1;term_row<num_terms;term_row++){
      if(pos_cofactor && input_cube[term_row][var_x]==neg){// for x=1 when apperance is complement just remove the cube
         for(int var_col=0;var_col<num_vars;var_col++) output_cube[term_row][var_col]=dont_exist;
      } else if(pos_cofactor && input_cube[term_row][var_x]==pos) {
         output_cube[term_row][var_x]=dont_care;
      } else if(!pos_cofactor && input_cube[term_row][var_x]==pos){
         for(int var_col=0;var_col<num_vars;var_col++) output_cube[term_row][var_col]=dont_exist;
      } else if(!pos_cofactor && input_cube[term_row][var_x]==neg) {
         output_cube[term_row][var_x]=dont_care;
      }
   }
   printf("%s cofactor for var_x %d\n",pos_cofactor?"positive":"negative",var_x); 
   //1.Empty cube list or one cube with all dont care
   int dont_exist_score=0;int dont_care_score=0;bool all_dont_care_row=false;int **new_output_cube=NULL;
   int num_terms_final=0;
   
   for(int term_row=1;term_row<num_terms;term_row++){
      for(int var_col=0;var_col<num_vars;var_col++) {
         if(output_cube[term_row][var_col]==dont_exist) dont_exist_score++;
         if(output_cube[term_row][var_col]==dont_care) dont_care_score++;
      }
      if(dont_care_score==num_vars){
         all_dont_care_row=true;break;
      }
      dont_care_score=0;//dont_care_score is row wise
   } 
   if(dont_exist_score==(num_terms-1)*num_vars) {
      //It is a 0 cube so send back a 1 i.e cube with all dont_care
      new_output_cube=allocate_pcn(num_vars,1);
      num_terms_final=new_output_cube[0][1];
      for(int term_row=1;term_row<num_terms_final;term_row++){
         for(int var_col=0;var_col<num_vars;var_col++) {
            new_output_cube[term_row][var_col]=dont_exist;
         }
      } 
      printf("Cofactor Case of empty cube list\n");
      print_pcn(new_output_cube);
      deallocate_pcn(output_cube);
      return new_output_cube;
   }   

   if(all_dont_care_row) {
      //It is a all 1 cube so send back a 0 i.e cube with all dont_exist
      new_output_cube=allocate_pcn(num_vars,1);
      num_terms_final=new_output_cube[0][1];
      for(int term_row=1;term_row<num_terms_final;term_row++){
         for(int var_col=0;var_col<num_vars;var_col++) {
            new_output_cube[term_row][var_col]=dont_care;
         }
      } 
      printf("Cofactor Case of one row with all dont care\n");
      print_pcn(new_output_cube);
      deallocate_pcn(output_cube);
      return new_output_cube;
   }   

   print_pcn(output_cube);
   return output_cube;
   
}
//Function to find cofactor

//Function to find complement of single row PCN
int **complement_single_row_pcn(int **cube,int index_single_row){
   int **final_cube_comp=NULL;
   int num_vars=cube[0][0];
   int num_terms=cube[0][1];
   int num_vars_pos_neg=0;
   for(int var_col=0;var_col<num_vars;var_col++) 
      if(cube[index_single_row][var_col]==pos || cube[index_single_row][var_col]==neg) num_vars_pos_neg++;
   //Allocate a pcn with num_vars_pos_neg rows/terms
   final_cube_comp=allocate_pcn(num_vars,num_vars_pos_neg);
   for(int var_col=0,term_row_final=1;var_col<num_vars;var_col++) {
      if(cube[index_single_row][var_col]==dont_care) continue;
      final_cube_comp[term_row_final][var_col]=(cube[index_single_row][var_col]==pos)?neg:pos;
      term_row_final++;
   }
   return final_cube_comp;
}


//Final function for recursion
int **complement_urp(int **cube){
   printf("Entering complement_urp for %d times\n",num_times);num_times++;
   //**************************************
   //URP complement implementation        *
   //**************************************
   int num_vars=cube[0][0];
   int num_terms=cube[0][1];
   int **final_cube_comp=NULL;
   int num_vars_final=num_vars;
   int num_terms_final=0;
   //Check for terminal cases
   //1.Empty cube list or one cube with all dont care
   int dont_exist_score=0;int dont_care_score=0;bool all_dont_care_row=false;
   
   for(int term_row=1;term_row<num_terms;term_row++){
      for(int var_col=0;var_col<num_vars;var_col++) {
         if(cube[term_row][var_col]==dont_exist) dont_exist_score++;
         if(cube[term_row][var_col]==dont_care) dont_care_score++;
      }
      if(dont_care_score==num_vars){
         all_dont_care_row=true;break;
      }
      dont_care_score=0;//dont_care_score is row wise
   } 
   if(dont_exist_score==(num_terms-1)*num_vars) {
      //It is a 0 cube so send back a 1 i.e cube with all dont_care
      final_cube_comp=allocate_pcn(num_vars,1);
      num_terms_final=final_cube_comp[0][1];
      for(int term_row=1;term_row<num_terms_final;term_row++){
         for(int var_col=0;var_col<num_vars;var_col++) {
            final_cube_comp[term_row][var_col]=dont_care;
         }
      } 
      printf("Case of empty cube list\n");
      print_pcn(final_cube_comp);
      return final_cube_comp;
   }   

   if(all_dont_care_row) {
      //It is a all 1 cube so send back a 0 i.e cube with all dont_exist
      final_cube_comp=allocate_pcn(num_vars,1);
      num_terms_final=final_cube_comp[0][1];
      for(int term_row=1;term_row<num_terms_final;term_row++){
         for(int var_col=0;var_col<num_vars;var_col++) {
            final_cube_comp[term_row][var_col]=dont_exist;
         }
      } 
      printf("Case of one row with all dont care\n");
      print_pcn(final_cube_comp);
      return final_cube_comp;
   }   
   //3. Check if single cube list PCN

   int rows_with_term=0;int index_single_row=-1;
   for(int term_row=1;term_row<num_terms;term_row++){
      for(int var_col=0;var_col<num_vars;var_col++) {
         if(cube[term_row][var_col]==pos || cube[term_row][var_col]==neg){ index_single_row=term_row;rows_with_term++;break; } 
      }
   } 
   if(rows_with_term==1) { //case of single cube list PCN
      final_cube_comp=complement_single_row_pcn(cube,index_single_row); 
      printf("Case of a single cube in pcn\n");
      print_pcn(final_cube_comp);
      return final_cube_comp;
   }



   
   //Check for terminal cases




   printf("Have to go for cofactors\n");
   //Have to go to cofcators
   int var_x=find_most_binate_variable(cube);
   int **pos_cube=NULL;int **neg_cube=NULL;bool pos_cofactor=true;
   int **pos_cube_comp=NULL;int **neg_cube_comp=NULL;
   pos_cube=find_cofactor(cube,var_x,pos_cofactor);
   neg_cube=find_cofactor(cube,var_x,!pos_cofactor);
   pos_cube_comp=complement_urp(pos_cube);
   neg_cube_comp=complement_urp(neg_cube);
   and_var_pcn(pos_cube_comp,var_x,pos);
   printf("After AND with pos term\n");
   print_pcn(pos_cube_comp);
   and_var_pcn(neg_cube_comp,var_x,neg);
   printf("After AND with neg term\n");
   print_pcn(neg_cube_comp);
   final_cube_comp=or_pcn(pos_cube_comp,neg_cube_comp,var_x);
   printf("After expansion varification\n");
   print_pcn(final_cube_comp); 
   deallocate_pcn(pos_cube);
   deallocate_pcn(neg_cube);
   deallocate_pcn(pos_cube_comp);
   deallocate_pcn(neg_cube_comp);
   return final_cube_comp;




   
   //**************************************
   //URP complement implementation        *
   //**************************************
}   




main(int argc,char *argv[])
{

   //**************************************
   //File Reading and Cube formation      *
   //**************************************
   //Data structure for file reading
   FILE *fp;
   char buff[255];
   char *buff_ptr=buff;
   memset(buff_ptr, '\0', sizeof( buff) );
   int num_vars,num_terms;
   enum line_index {num_vars_line_index,num_terms_line_index,actual_cube_line_index};   
   int line_count=0;
   int num_terms_each_line=0;
   //Data structure for file reading
   
   //Data structure for cube population
   //Lets nt do it bitwise
   //In PCN int 1->positive -1->negative 0->dont care 4->00
   int **cube=NULL;
   int var_col=0;int term_row=0; 
   //Data structure for cube population

   //File reading
   fp = fopen(argv[1], "r");
   if( fp != NULL ){
      while (fgets(buff_ptr, 255, fp)){
         printf("Line no. %d: %s",line_count,buff_ptr );
         var_col=0;//new line new variables from start
         int arg, offset; 
         while(1==sscanf((const char *)buff_ptr,"%d%n",&arg,&offset)){
            //printf("arg is %d\n",arg);
            if(line_count==num_vars_line_index){
               num_vars=arg;
            }
            if(line_count==num_terms_line_index){
               num_terms=arg;
               cube=allocate_pcn(num_vars,num_terms);
               //cube=(int **)calloc(num_terms,sizeof(int *));
               //for(int row_index=0;row_index<num_terms;row_index++){ 
                  //cube[row_index]=(int *)calloc(num_vars,sizeof(int));
               //}
            }
            if(line_count>=actual_cube_line_index){
               //populate_cube;
               if(num_terms_each_line==0) {num_terms_each_line=arg;buff_ptr+=offset;continue;}
               term_row=line_count-2+1;
               //printf("term_row %d var_col %d arg %d\n",term_row,var_col,arg);
               if(arg>0){ //positive term
                  var_col=arg-1;
                  cube[term_row][var_col]=pos;
               }else{
                  var_col=abs(arg)-1;
                  cube[term_row][var_col]=neg;
               } 
            }
            
            buff_ptr+=offset;
         }   
         line_count++;
         buff_ptr=buff;
         num_terms_each_line=0;
         memset(buff_ptr, '\0', sizeof( buff) );
      }
      fclose(fp);
   }
   print_pcn(cube);
   int **final_cube_comp=NULL;
   final_cube_comp=complement_urp(cube);
   print_pcn(final_cube_comp);
   printf("In final output format\n");
   print_pcn_output_format(final_cube_comp);
   deallocate_pcn(cube);
   deallocate_pcn(final_cube_comp);
   //**************************************
   //File Reading and Cube formation      *
   //**************************************
}



   



