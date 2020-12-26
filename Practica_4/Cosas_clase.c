void movimiento()
{
if (valor!=0) 
  {
   if (flag==0) tanque.giro_tubo-=valor;
   if (tanque.giro_tubo<tanque.giro_tubo_min && flag==0)
       {tanque.giro_tubo=tanque.giro_tubo_min;
        flag=1;
       }
   if (flag==1) tanque.giro_tubo+=valor;
   if (tanque.giro_tubo>tanque.giro_tubo_max && flag==1)   	
       {tanque.giro_tubo=tanque.giro_tubo_max;
        flag=0;
       }
   glutPostRedisplay();
  }
}