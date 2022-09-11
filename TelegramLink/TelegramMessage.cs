using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Telegram
{
    public class TelegramMessage
    {
        public int chatId;
        public string chatType;
        public string userFirstName;
        public string username;
        public string text;
        public DateTime date;
        public int __updateId;
    }
}
