using JsonMaker;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Telegram
{
    public delegate void OnMessage(TelegramMessage message);
    class TelegramBot
    {
        private string APIKey;
        private string TelegramBotUrl = "https://api.telegram.org/";
        private bool running = false;
        private Thread threadGetUpdatesPointer;
        private int lastUpdateId = 0;
        private int lastViewedId = 0;
        private int verificationInterval = 2000;

        public OnMessage onMessage;
        public TelegramBot(string APIKey, OnMessage onMessage, int verificationInterval = 2000)
        {
            this.verificationInterval = verificationInterval;
            this.APIKey = APIKey;
            TelegramBotUrl += "bot" + APIKey + "/";
            this.onMessage = onMessage;

            this.threadGetUpdatesPointer = new Thread(this.threadGetUpdates);
            this.threadGetUpdatesPointer.Start();

        }

        public void sendMessage(string text, int chatId)
        {
            text = text.Replace("\"", "\\\"");
            string response = httpRequest(this.TelegramBotUrl + "sendMessage", "POST", "{\"chat_id\":\"" + chatId + "\", \"text\":\""+text+"\"}", new Dictionary<string, string> { { "content-type", "application/json" } } );
            
        }
        private void threadGetUpdates()
        {
            this.running = true;
            string response = "";
            List<TelegramMessage> receivedMessages = new List<TelegramMessage>();
            TelegramMessage currentMessage;
            while (running)
            {
                try
                {
                    receivedMessages.Clear();
                    response = httpRequest(this.TelegramBotUrl + "getUpdates", "POST", "{\"offset\":\"" + (this.lastUpdateId + 1).ToString() + "\"}", new Dictionary<string, string> { { "content-type", "application/json" } });
                    //importa os dados JSON para o processador
                    JSON jm = new JSON(response);
                    if (jm.getBoolean("ok"))
                    {
                        int messagesCount = jm.getArrayLength("result");
                        for (int count = 0; count < messagesCount; count ++)
                        {
                            this.lastUpdateId = jm.getInt("result["+count+"].update_id");

                            currentMessage = new TelegramMessage();
                            currentMessage.__updateId = this.lastUpdateId;
                            currentMessage.userFirstName = jm.getString("result[" + count + "].message.from.first_name");
                            currentMessage.username = jm.getString("result[" + count + "].message.from.username");
                            currentMessage.chatId = jm.getInt("result[" + count + "].message.chat.id");
                            currentMessage.chatType = jm.getString("result[" + count + "].message.chat.type");
                            currentMessage.date = (new DateTime(1970,1,1,0,0,0,0)).AddSeconds(jm.getInt("result[" + count + "].message.date"));
                            currentMessage.text = jm.getString("result[" + count + "].message.text");
                            
                            this.processReceivedMessage(currentMessage);
                        }
                    }

                }
                catch (Exception e) { }
                Thread.Sleep(verificationInterval);
            }
        }

        private void processReceivedMessage(TelegramMessage message)
        {

            if (message.__updateId > lastViewedId)
            {
                if (onMessage != null)
                {
                    lastViewedId = message.__updateId;
                    this.onMessage(message);
                }
                
            }

        }

        public string httpRequest(string url, string method = "GET", string postData = "", Dictionary<string, string> headers = null)
        {
            HttpClient client = new HttpClient();

            method = method.ToUpper();


            string mimeType = "text/plain";

            if (headers != null)
            {
                foreach (var c in headers)
                {
                    if ((c.Key.ToLower() == "content-type") || (c.Key.ToLower() == "accept"))
                    {
                        client.DefaultRequestHeaders.Accept.Add(new System.Net.Http.Headers.MediaTypeWithQualityHeaderValue(c.Value));
                        mimeType = c.Value;
                    }
                    else
                        client.DefaultRequestHeaders.TryAddWithoutValidation(c.Key, c.Value);

                }
            }


            

            Task<HttpResponseMessage> wait1 = null;
            if (method == "GET")
                wait1 = client.GetAsync(url);
            if (method == "POST")
                wait1 = client.PostAsync(url, new StringContent(postData, Encoding.UTF8, mimeType));
            if (method == "DELETE")
                wait1 = client.DeleteAsync(url);
            if (method == "PUT")
                wait1 = client.PutAsync(url, new StringContent(postData));


            wait1.Wait();
            var response = wait1.Result;
            

            //takes the server response
            var wait2 = response.Content.ReadAsStringAsync();
            wait2.Wait();
            string result = wait2.Result;

            return result;

        }
    }
}
