using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Networking;
using System.Threading.Tasks;
using Newtonsoft.Json;
using TMPro;

public class GetData : MonoBehaviour
{   
    public int minutes;
    public string date;

    public async void  Start()
    {

        var url = "http://127.0.0.1:8000//activity";
        //var url = "https://werkverzeichnis.orijauch.de/today";
         var www = UnityWebRequest.Get(url);
        www.SetRequestHeader("Content-Type","application/json");
        var operation= www.SendWebRequest();
        while(!operation.isDone)
            await Task.Yield();
        var jsonResponse = www.downloadHandler.text;
        //Debug.Log(www.downloadHandler.text);

        if (www.result== UnityWebRequest.Result.Success)
          {

          var res = JsonConvert.DeserializeObject<User>(jsonResponse);
     
          minutes= res.minutes;
          //date=res.date;

      
          }

    
    }

      // Update is called once per frame
      public  void Update()
      {
        
           
      }

      public int  ReturnData()
    {
            //await Task.Run(() =>Start());
            return minutes;
    }   
}