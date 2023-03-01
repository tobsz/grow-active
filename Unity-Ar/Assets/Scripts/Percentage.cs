using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using System;

public class Percentage : MonoBehaviour
{
    public TMP_Text canvas;
    public GetData getdata;
    private int minutes;
    private int frames =100;
    private double  Total_percentage=0;
    public double percentage=0;
    public double percentage_now;


    // Start is called before the first frame update
    void Start()
    {


    }

    // Update is called once per frame
    void Update(){

      if (percentage<Total_percentage){
            percentage+= 1;
            canvas.text=percentage.ToString() +"%";
        }

    if (percentage>Total_percentage){
            percentage-= 1;
            canvas.text=percentage.ToString() +"%";
        }
        }


    public  void update_percentage()
    {
        StartCoroutine(Data_Coroutine());
        IEnumerator Data_Coroutine()
    {
        getdata.Start();
        yield return new WaitForSeconds(1);
        minutes= getdata.ReturnData() ;
        Total_percentage=Math.Round(minutes/30.0f *100);
       
    }


    }
    public void reset_percentage(){
        Total_percentage=0;

        minutes=0;

   }
    
}
