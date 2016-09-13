// junand 07.09.2016

#ifndef JSON_TEST_H
#define JSON_TEST_H
/***
const char* httpCurrentWestherResponseTest = "\
{\
    \"coord\":{\"lon\":13.54,\"lat\":52.65},\
    \"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"01d\"}],\
    \"base\":\"stations\",\
    \"main\":{\"temp\":296.48,\"pressure\":1017,\"humidity\":52,\"temp_min\":294.15,\"temp_max\":303.15},\
    \"visibility\":10000,\
    \"wind\":{\"speed\":4.1,\"deg\":290},\
    \"clouds\":{\"all\":0},\
    \"dt\":1472891622,\
    \"sys\":{\"type\":1,\"id\":4892,\"message\":0.0347,\"country\":\"DE\",\"sunrise\":1472876456,\"sunset\":1472924870},\
    \"id\":2804759,\
    \"name\":\"Zepernick\",\
    \"cod\":200\
}\
";
*/
/***
const char* httpForecastWestherResponseTest_de = "\
{\
  \"city\": {\
    \"id\": 2804759,\
    \"name\": \"Zepernick\",\
    \"coord\": {\
      \"lon\": 13.54091,\
      \"lat\": 52.650871\
    },\
    \"country\": \"DE\",\
    \"population\": 0,\
    \"sys\": {\
      \"population\": 0\
    }\
  },\
  \"cod\": \"200\",\
  \"message\": 0.0029,\
  \"cnt\": 39,\
  \"list\": [\
    {\
      \"dt\": 1473228000,\
      \"main\": {\
        \"temp\": 13.29,\
        \"temp_min\": 11.62,\
        \"temp_max\": 13.29,\
        \"pressure\": 1033.63,\
        \"sea_level\": 1039.47,\
        \"grnd_level\": 1033.63,\
        \"humidity\": 84,\
        \"temp_kf\": 1.67\
      },\
      \"weather\": [\
        {\
          \"id\": 801,\
          \"main\": \"Clouds\",\
          \"description\": \"ein paar Wolken\",\
          \"icon\": \"02d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 20\
      },\
      \"wind\": {\
        \"speed\": 1.34,\
        \"deg\": 146.503\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-07 06:00:00\"\
    },\
    {\
      \"dt\": 1473238800,\
      \"main\": {\
        \"temp\": 20.82,\
        \"temp_min\": 19.24,\
        \"temp_max\": 20.82,\
        \"pressure\": 1033.22,\
        \"sea_level\": 1039,\
        \"grnd_level\": 1033.22,\
        \"humidity\": 88,\
        \"temp_kf\": 1.58\
      },\
      \"weather\": [\
        {\
          \"id\": 802,\
          \"main\": \"Clouds\",\
          \"description\": \"überwiegend bewölkt\",\
          \"icon\": \"03d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 48\
      },\
      \"wind\": {\
        \"speed\": 2.17,\
        \"deg\": 165.504\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-07 09:00:00\"\
    },\
    {\
      \"dt\": 1473249600,\
      \"main\": {\
        \"temp\": 22.91,\
        \"temp_min\": 21.42,\
        \"temp_max\": 22.91,\
        \"pressure\": 1031.85,\
        \"sea_level\": 1037.48,\
        \"grnd_level\": 1031.85,\
        \"humidity\": 78,\
        \"temp_kf\": 1.49\
      },\
      \"weather\": [\
        {\
          \"id\": 801,\
          \"main\": \"Clouds\",\
          \"description\": \"ein paar Wolken\",\
          \"icon\": \"02d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 24\
      },\
      \"wind\": {\
        \"speed\": 2.46,\
        \"deg\": 159\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-07 12:00:00\"\
    },\
  ]\
}\
";
*/
/**
const char* httpForecastWestherResponseTest_en = "\
{\
  \"city\": {\
    \"id\": 2804759,\
    \"name\": \"Zepernick\",\
    \"coord\": {\
      \"lon\": 13.54091,\
      \"lat\": 52.650871\
    },\
    \"country\": \"DE\",\
    \"population\": 0,\
    \"sys\": {\
      \"population\": 0\
    }\
  },\
  \"cod\": \"200\",\
  \"message\": 0.0043,\
  \"cnt\": 40,\
  \"list\": [\
    {\
      \"dt\": 1473454800,\
      \"main\": {\
        \"temp\": 16.17,\
        \"temp_min\": 16.17,\
        \"temp_max\": 17.79,\
        \"pressure\": 1026.13,\
        \"sea_level\": 1031.78,\
        \"grnd_level\": 1026.13,\
        \"humidity\": 72,\
        \"temp_kf\": -1.62\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 3.47,\
        \"deg\": 344.001\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-09 21:00:00\"\
    },\
    {\
      \"dt\": 1473465600,\
      \"main\": {\
        \"temp\": 12.19,\
        \"temp_min\": 12.19,\
        \"temp_max\": 13.72,\
        \"pressure\": 1026.37,\
        \"sea_level\": 1032.18,\
        \"grnd_level\": 1026.37,\
        \"humidity\": 87,\
        \"temp_kf\": -1.53\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 1.31,\
        \"deg\": 339.503\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-10 00:00:00\"\
    },\
    {\
      \"dt\": 1473476400,\
      \"main\": {\
        \"temp\": 10.65,\
        \"temp_min\": 10.65,\
        \"temp_max\": 12.09,\
        \"pressure\": 1026.47,\
        \"sea_level\": 1032.34,\
        \"grnd_level\": 1026.47,\
        \"humidity\": 86,\
        \"temp_kf\": -1.44\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 1.26,\
        \"deg\": 13.501\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-10 03:00:00\"\
    },\
    {\
      \"dt\": 1473487200,\
      \"main\": {\
        \"temp\": 13.38,\
        \"temp_min\": 13.38,\
        \"temp_max\": 14.73,\
        \"pressure\": 1026.72,\
        \"sea_level\": 1032.47,\
        \"grnd_level\": 1026.72,\
        \"humidity\": 85,\
        \"temp_kf\": -1.35\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 1.31,\
        \"deg\": 63.5003\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-10 06:00:00\"\
    },\
    {\
      \"dt\": 1473498000,\
      \"main\": {\
        \"temp\": 21.95,\
        \"temp_min\": 21.95,\
        \"temp_max\": 23.21,\
        \"pressure\": 1026.51,\
        \"sea_level\": 1032.16,\
        \"grnd_level\": 1026.51,\
        \"humidity\": 59,\
        \"temp_kf\": -1.26\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"02d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 8\
      },\
      \"wind\": {\
        \"speed\": 2.1,\
        \"deg\": 111.5\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-10 09:00:00\"\
    },\
    {\
      \"dt\": 1473508800,\
      \"main\": {\
        \"temp\": 25.74,\
        \"temp_min\": 25.74,\
        \"temp_max\": 26.91,\
        \"pressure\": 1025.43,\
        \"sea_level\": 1031,\
        \"grnd_level\": 1025.43,\
        \"humidity\": 51,\
        \"temp_kf\": -1.17\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 2.24,\
        \"deg\": 142.504\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-10 12:00:00\"\
    },\
    {\
      \"dt\": 1473519600,\
      \"main\": {\
        \"temp\": 26.95,\
        \"temp_min\": 26.95,\
        \"temp_max\": 28.03,\
        \"pressure\": 1023.73,\
        \"sea_level\": 1029.32,\
        \"grnd_level\": 1023.73,\
        \"humidity\": 47,\
        \"temp_kf\": -1.08\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 2.59,\
        \"deg\": 155.5\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-10 15:00:00\"\
    },\
    {\
      \"dt\": 1473530400,\
      \"main\": {\
        \"temp\": 22.76,\
        \"temp_min\": 22.76,\
        \"temp_max\": 23.75,\
        \"pressure\": 1022.95,\
        \"sea_level\": 1028.64,\
        \"grnd_level\": 1022.95,\
        \"humidity\": 54,\
        \"temp_kf\": -0.99\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 2.57,\
        \"deg\": 122.503\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-10 18:00:00\"\
    },\
    {\
      \"dt\": 1473541200,\
      \"main\": {\
        \"temp\": 19.88,\
        \"temp_min\": 19.88,\
        \"temp_max\": 20.77,\
        \"pressure\": 1022.84,\
        \"sea_level\": 1028.56,\
        \"grnd_level\": 1022.84,\
        \"humidity\": 59,\
        \"temp_kf\": -0.9\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 4.01,\
        \"deg\": 145.001\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-10 21:00:00\"\
    },\
    {\
      \"dt\": 1473552000,\
      \"main\": {\
        \"temp\": 18.25,\
        \"temp_min\": 18.25,\
        \"temp_max\": 19.06,\
        \"pressure\": 1023.1,\
        \"sea_level\": 1028.84,\
        \"grnd_level\": 1023.1,\
        \"humidity\": 72,\
        \"temp_kf\": -0.81\
      },\
      \"weather\": [\
        {\
          \"id\": 801,\
          \"main\": \"Clouds\",\
          \"description\": \"few clouds\",\
          \"icon\": \"02n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 12\
      },\
      \"wind\": {\
        \"speed\": 2.76,\
        \"deg\": 166\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-11 00:00:00\"\
    },\
    {\
      \"dt\": 1473562800,\
      \"main\": {\
        \"temp\": 16.89,\
        \"temp_min\": 16.89,\
        \"temp_max\": 17.61,\
        \"pressure\": 1023.76,\
        \"sea_level\": 1029.38,\
        \"grnd_level\": 1023.76,\
        \"humidity\": 83,\
        \"temp_kf\": -0.72\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 2.52,\
        \"deg\": 233.003\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-11 03:00:00\"\
    },\
    {\
      \"dt\": 1473573600,\
      \"main\": {\
        \"temp\": 17.43,\
        \"temp_min\": 17.43,\
        \"temp_max\": 18.06,\
        \"pressure\": 1024.68,\
        \"sea_level\": 1030.37,\
        \"grnd_level\": 1024.68,\
        \"humidity\": 81,\
        \"temp_kf\": -0.63\
      },\
      \"weather\": [\
        {\
          \"id\": 801,\
          \"main\": \"Clouds\",\
          \"description\": \"few clouds\",\
          \"icon\": \"02d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 20\
      },\
      \"wind\": {\
        \"speed\": 1.31,\
        \"deg\": 256.002\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-11 06:00:00\"\
    },\
    {\
      \"dt\": 1473584400,\
      \"main\": {\
        \"temp\": 24.52,\
        \"temp_min\": 24.52,\
        \"temp_max\": 25.06,\
        \"pressure\": 1026.03,\
        \"sea_level\": 1031.72,\
        \"grnd_level\": 1026.03,\
        \"humidity\": 49,\
        \"temp_kf\": -0.54\
      },\
      \"weather\": [\
        {\
          \"id\": 801,\
          \"main\": \"Clouds\",\
          \"description\": \"few clouds\",\
          \"icon\": \"02d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 12\
      },\
      \"wind\": {\
        \"speed\": 2.2,\
        \"deg\": 268.003\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-11 09:00:00\"\
    },\
    {\
      \"dt\": 1473595200,\
      \"main\": {\
        \"temp\": 26.94,\
        \"temp_min\": 26.94,\
        \"temp_max\": 27.39,\
        \"pressure\": 1026.29,\
        \"sea_level\": 1031.94,\
        \"grnd_level\": 1026.29,\
        \"humidity\": 44,\
        \"temp_kf\": -0.45\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 2.96,\
        \"deg\": 263.501\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-11 12:00:00\"\
    },\
    {\
      \"dt\": 1473606000,\
      \"main\": {\
        \"temp\": 27.31,\
        \"temp_min\": 27.31,\
        \"temp_max\": 27.67,\
        \"pressure\": 1026.16,\
        \"sea_level\": 1031.88,\
        \"grnd_level\": 1026.16,\
        \"humidity\": 41,\
        \"temp_kf\": -0.36\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 2.87,\
        \"deg\": 284.501\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-11 15:00:00\"\
    },\
    {\
      \"dt\": 1473616800,\
      \"main\": {\
        \"temp\": 22.72,\
        \"temp_min\": 22.72,\
        \"temp_max\": 22.99,\
        \"pressure\": 1026.98,\
        \"sea_level\": 1032.8,\
        \"grnd_level\": 1026.98,\
        \"humidity\": 54,\
        \"temp_kf\": -0.27\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"02n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 8\
      },\
      \"wind\": {\
        \"speed\": 2.56,\
        \"deg\": 329.001\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-11 18:00:00\"\
    },\
    {\
      \"dt\": 1473627600,\
      \"main\": {\
        \"temp\": 19.58,\
        \"temp_min\": 19.58,\
        \"temp_max\": 19.76,\
        \"pressure\": 1028.05,\
        \"sea_level\": 1033.86,\
        \"grnd_level\": 1028.05,\
        \"humidity\": 73,\
        \"temp_kf\": -0.18\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 3.11,\
        \"deg\": 1.01871\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-11 21:00:00\"\
    },\
    {\
      \"dt\": 1473638400,\
      \"main\": {\
        \"temp\": 17.63,\
        \"temp_min\": 17.63,\
        \"temp_max\": 17.72,\
        \"pressure\": 1028.71,\
        \"sea_level\": 1034.46,\
        \"grnd_level\": 1028.71,\
        \"humidity\": 83,\
        \"temp_kf\": -0.09\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 3.56,\
        \"deg\": 46.506\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-12 00:00:00\"\
    },\
    {\
      \"dt\": 1473649200,\
      \"main\": {\
        \"temp\": 14.95,\
        \"temp_min\": 14.95,\
        \"temp_max\": 14.95,\
        \"pressure\": 1028.58,\
        \"sea_level\": 1034.39,\
        \"grnd_level\": 1028.58,\
        \"humidity\": 92,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 1.66,\
        \"deg\": 44.0038\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-12 03:00:00\"\
    },\
    {\
      \"dt\": 1473660000,\
      \"main\": {\
        \"temp\": 16.41,\
        \"temp_min\": 16.41,\
        \"temp_max\": 16.41,\
        \"pressure\": 1028.59,\
        \"sea_level\": 1034.38,\
        \"grnd_level\": 1028.59,\
        \"humidity\": 85,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 1.87,\
        \"deg\": 74.0026\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-12 06:00:00\"\
    },\
    {\
      \"dt\": 1473670800,\
      \"main\": {\
        \"temp\": 25.16,\
        \"temp_min\": 25.16,\
        \"temp_max\": 25.16,\
        \"pressure\": 1028.92,\
        \"sea_level\": 1034.65,\
        \"grnd_level\": 1028.92,\
        \"humidity\": 51,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 2.96,\
        \"deg\": 104.501\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-12 09:00:00\"\
    },\
    {\
      \"dt\": 1473681600,\
      \"main\": {\
        \"temp\": 29.43,\
        \"temp_min\": 29.43,\
        \"temp_max\": 29.43,\
        \"pressure\": 1028.2,\
        \"sea_level\": 1033.79,\
        \"grnd_level\": 1028.2,\
        \"humidity\": 42,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 2.06,\
        \"deg\": 147.501\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-12 12:00:00\"\
    },\
    {\
      \"dt\": 1473692400,\
      \"main\": {\
        \"temp\": 29.84,\
        \"temp_min\": 29.84,\
        \"temp_max\": 29.84,\
        \"pressure\": 1027.18,\
        \"sea_level\": 1032.73,\
        \"grnd_level\": 1027.18,\
        \"humidity\": 38,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 1.93,\
        \"deg\": 150.001\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-12 15:00:00\"\
    },\
    {\
      \"dt\": 1473703200,\
      \"main\": {\
        \"temp\": 24.08,\
        \"temp_min\": 24.08,\
        \"temp_max\": 24.08,\
        \"pressure\": 1027,\
        \"sea_level\": 1032.7,\
        \"grnd_level\": 1027,\
        \"humidity\": 62,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 1.41,\
        \"deg\": 65.502\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-12 18:00:00\"\
    },\
    {\
      \"dt\": 1473714000,\
      \"main\": {\
        \"temp\": 20.34,\
        \"temp_min\": 20.34,\
        \"temp_max\": 20.34,\
        \"pressure\": 1027.28,\
        \"sea_level\": 1032.99,\
        \"grnd_level\": 1027.28,\
        \"humidity\": 59,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 2.76,\
        \"deg\": 59.5006\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-12 21:00:00\"\
    },\
    {\
      \"dt\": 1473724800,\
      \"main\": {\
        \"temp\": 17.62,\
        \"temp_min\": 17.62,\
        \"temp_max\": 17.62,\
        \"pressure\": 1027.35,\
        \"sea_level\": 1033.11,\
        \"grnd_level\": 1027.35,\
        \"humidity\": 80,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 1.26,\
        \"deg\": 86.5043\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-13 00:00:00\"\
    },\
    {\
      \"dt\": 1473735600,\
      \"main\": {\
        \"temp\": 15.89,\
        \"temp_min\": 15.89,\
        \"temp_max\": 15.89,\
        \"pressure\": 1027.13,\
        \"sea_level\": 1032.97,\
        \"grnd_level\": 1027.13,\
        \"humidity\": 84,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 1.26,\
        \"deg\": 55.5032\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-13 03:00:00\"\
    },\
    {\
      \"dt\": 1473746400,\
      \"main\": {\
        \"temp\": 17.53,\
        \"temp_min\": 17.53,\
        \"temp_max\": 17.53,\
        \"pressure\": 1027.25,\
        \"sea_level\": 1032.98,\
        \"grnd_level\": 1027.25,\
        \"humidity\": 81,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 1.28,\
        \"deg\": 50.001\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-13 06:00:00\"\
    },\
    {\
      \"dt\": 1473757200,\
      \"main\": {\
        \"temp\": 26.87,\
        \"temp_min\": 26.87,\
        \"temp_max\": 26.87,\
        \"pressure\": 1027.46,\
        \"sea_level\": 1033.16,\
        \"grnd_level\": 1027.46,\
        \"humidity\": 49,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 2.22,\
        \"deg\": 95.5018\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-13 09:00:00\"\
    },\
    {\
      \"dt\": 1473768000,\
      \"main\": {\
        \"temp\": 30.87,\
        \"temp_min\": 30.87,\
        \"temp_max\": 30.87,\
        \"pressure\": 1026.43,\
        \"sea_level\": 1032.13,\
        \"grnd_level\": 1026.43,\
        \"humidity\": 40,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 2.52,\
        \"deg\": 105.001\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-13 12:00:00\"\
    },\
    {\
      \"dt\": 1473778800,\
      \"main\": {\
        \"temp\": 30.77,\
        \"temp_min\": 30.77,\
        \"temp_max\": 30.77,\
        \"pressure\": 1025.32,\
        \"sea_level\": 1030.94,\
        \"grnd_level\": 1025.32,\
        \"humidity\": 36,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 3.02,\
        \"deg\": 101.502\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-13 15:00:00\"\
    },\
    {\
      \"dt\": 1473789600,\
      \"main\": {\
        \"temp\": 25.62,\
        \"temp_min\": 25.62,\
        \"temp_max\": 25.62,\
        \"pressure\": 1025.12,\
        \"sea_level\": 1030.79,\
        \"grnd_level\": 1025.12,\
        \"humidity\": 45,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 3.03,\
        \"deg\": 67.0033\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-13 18:00:00\"\
    },\
    {\
      \"dt\": 1473800400,\
      \"main\": {\
        \"temp\": 22.99,\
        \"temp_min\": 22.99,\
        \"temp_max\": 22.99,\
        \"pressure\": 1025.85,\
        \"sea_level\": 1031.49,\
        \"grnd_level\": 1025.85,\
        \"humidity\": 53,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 5.51,\
        \"deg\": 75.0018\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-13 21:00:00\"\
    },\
    {\
      \"dt\": 1473811200,\
      \"main\": {\
        \"temp\": 21.04,\
        \"temp_min\": 21.04,\
        \"temp_max\": 21.04,\
        \"pressure\": 1026.15,\
        \"sea_level\": 1031.96,\
        \"grnd_level\": 1026.15,\
        \"humidity\": 63,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 4.91,\
        \"deg\": 78.5014\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-14 00:00:00\"\
    },\
    {\
      \"dt\": 1473822000,\
      \"main\": {\
        \"temp\": 18.75,\
        \"temp_min\": 18.75,\
        \"temp_max\": 18.75,\
        \"pressure\": 1026.42,\
        \"sea_level\": 1032.18,\
        \"grnd_level\": 1026.42,\
        \"humidity\": 62,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 4.52,\
        \"deg\": 74.5042\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-14 03:00:00\"\
    },\
    {\
      \"dt\": 1473832800,\
      \"main\": {\
        \"temp\": 17.81,\
        \"temp_min\": 17.81,\
        \"temp_max\": 17.81,\
        \"pressure\": 1026.81,\
        \"sea_level\": 1032.5,\
        \"grnd_level\": 1026.81,\
        \"humidity\": 52,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 5.07,\
        \"deg\": 78.5001\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-14 06:00:00\"\
    },\
    {\
      \"dt\": 1473843600,\
      \"main\": {\
        \"temp\": 22.26,\
        \"temp_min\": 22.26,\
        \"temp_max\": 22.26,\
        \"pressure\": 1027.06,\
        \"sea_level\": 1032.72,\
        \"grnd_level\": 1027.06,\
        \"humidity\": 44,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 5.21,\
        \"deg\": 84.0013\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-14 09:00:00\"\
    },\
    {\
      \"dt\": 1473854400,\
      \"main\": {\
        \"temp\": 24.79,\
        \"temp_min\": 24.79,\
        \"temp_max\": 24.79,\
        \"pressure\": 1026.35,\
        \"sea_level\": 1032.05,\
        \"grnd_level\": 1026.35,\
        \"humidity\": 37,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"02d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 8\
      },\
      \"wind\": {\
        \"speed\": 5.11,\
        \"deg\": 80.5007\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-14 12:00:00\"\
    },\
    {\
      \"dt\": 1473865200,\
      \"main\": {\
        \"temp\": 24.66,\
        \"temp_min\": 24.66,\
        \"temp_max\": 24.66,\
        \"pressure\": 1025.42,\
        \"sea_level\": 1031.03,\
        \"grnd_level\": 1025.42,\
        \"humidity\": 32,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01d\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 5.32,\
        \"deg\": 75.5001\
      },\
      \"sys\": {\
        \"pod\": \"d\"\
      },\
      \"dt_txt\": \"2016-09-14 15:00:00\"\
    },\
    {\
      \"dt\": 1473876000,\
      \"main\": {\
        \"temp\": 20.54,\
        \"temp_min\": 20.54,\
        \"temp_max\": 20.54,\
        \"pressure\": 1025.16,\
        \"sea_level\": 1030.91,\
        \"grnd_level\": 1025.16,\
        \"humidity\": 38,\
        \"temp_kf\": 0\
      },\
      \"weather\": [\
        {\
          \"id\": 800,\
          \"main\": \"Clear\",\
          \"description\": \"clear sky\",\
          \"icon\": \"01n\"\
        }\
      ],\
      \"clouds\": {\
        \"all\": 0\
      },\
      \"wind\": {\
        \"speed\": 4.57,\
        \"deg\": 74.0005\
      },\
      \"sys\": {\
        \"pod\": \"n\"\
      },\
      \"dt_txt\": \"2016-09-14 18:00:00\"\
    }\
  ]\
}\
";
*/
#endif