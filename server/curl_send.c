#include <stdio.h>
#include <stdlib.h>
#include "curl_send.h"


void sendtoDiscord(double temp1, double temp2)
{
    static char dicsord_format[1024];

    snprintf(dicsord_format, sizeof(dicsord_format), "{\n"
  "  \"embeds\": [\n"
  "    {\n"
  "      \"title\": \"警告\",\n"
  "      \"description\": \"室温差, もしくは湯船の温度に問題があります。\\n浴室の使用者に注意してあげてください。\",\n"
  "      \"color\": 38166,\n"
  "      \"url\": \"\",\n"
  "      \"author\": {\n"
  "        \"name\": \"Carnot\"\n"
  "      },\n"
  "      \"footer\": {\n"
  "        \"text\": \"Carnot\"\n"
  "      },\n"
  "      \"fields\": [\n"
  "        {\n"
  "          \"name\": \"室温1(リビング)\",\n"
  "          \"value\": \"%0.2f°C\",\n"
  "          \"inline\": true\n"
  "        },\n"
  "        {\n"
  "          \"name\": \"室温2(脱衣所)\",\n"
  "          \"value\": \"%0.2f°C\",\n"
  "          \"inline\": true\n"
  "        }\n"
  "      ]\n"
  "    }\n"
  "  ]\n"
  "}", temp1, temp2);
    char request[2048];
    snprintf(request, sizeof(request), "curl -H \"Content-Type: application/json\" -X POST -d '%s' \"%s\"", dicsord_format, DISCORD_URL);

    system(request);
}
