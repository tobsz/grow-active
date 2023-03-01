using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ProgressBar : MonoBehaviour
{
    public GetData getdata;
    private Slider slider;
    private float targetProgress=0;
    private float fillSpeed= 0.8f;
    private int minutes;
    float newProgress;
    bool update;

    private void Awake()
    {
        slider = gameObject.GetComponent<Slider>();
        
    }
    // Start is called before the first frame update
    public void Start()
    {
       
    }

    public void Update(){

     
        slider.gameObject.transform.Find("Fill Area").Find("Fill").GetComponent<Image>().color=new Color(255f/255f, 0f/255f, 0f/255f);

        //slider.gameObject.transform.Find("Fill Area").Find("Fill").GetComponent<Image>().color=Color.Lerp(new Color(245f/255f, 27f/255f, 27/255f),new Color(0f/255f, 255f/255f, 0f/255f),slider.value);
        if (slider.value>targetProgress)
        {
            slider.value-=fillSpeed*Time.deltaTime;        
        }

        if (targetProgress==0){
            slider.gameObject.transform.Find("Fill Area").Find("Fill").GetComponent<Image>().color=new Color(255f/255f, 0f/255f, 0f/255f);
        }

        else if (targetProgress<=0.35) {
             slider.gameObject.transform.Find("Fill Area").Find("Fill").GetComponent<Image>().color=new Color(227f/255f, 191f/255f, 99f/255f);
        }
        else if (targetProgress<=0.8){
            slider.gameObject.transform.Find("Fill Area").Find("Fill").GetComponent<Image>().color=new Color(99f/255f, 204f/255f, 227f/255f);
        }
        else{
            slider.gameObject.transform.Find("Fill Area").Find("Fill").GetComponent<Image>().color=new Color(99f/255f, 227f/255f, 150f/255f);
        }
        if (slider.value<targetProgress)
        {

            slider.value += fillSpeed * Time.deltaTime;
        }


    }

    //void IncrementProgress(float newProgress)
    public void IncrementProgress()
    {
        StartCoroutine(ExampleCoroutine());
         IEnumerator ExampleCoroutine()
    {
        getdata.Start();
        yield return new WaitForSeconds(1);
        newProgress= getdata.ReturnData()/30.0f;
        //Debug.Log(newProgress);
        targetProgress=slider.value+newProgress;
        //yield return new WaitForSeconds(1);   
    }

    }
    public void DecrementProgress(){
        targetProgress=0;
        newProgress=0;
    }
}
