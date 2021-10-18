<h1  align="center">
<img  src="https://images.squarespace-cdn.com/content/v1/600f8dac4894874fe905dce4/1623280611899-I09906CEGN5934NGA6HE/logo_transparent_white.png?format=1500w"  alt="CLIMATECLOCK"  width="300"/>

# Climate Clock Repo

</h1>
Encontra aqui as diferentes formas de modificares este código para o teu climate clock usando um Arduino! Este codigo foi feito para numa matriz de LED's controlada por um Arduino apresentar o tempo restante para parar o aquecimento global!

<img  src="https://prnt.sc/1wloyy8"  alt="ClimateClockafuncionar"  width="300"/>

# Tutoriais:

_O que podes fazer com este repositório?_

- ### Instalação
  - Para instalares esta versão vais precisar de escolher uma das duas pastas no repositório. Uma delas _(Codigo_outro_RTC)_ está adaptada para RTC's do tipo: **"DS1307", "DS1307", "DS3231", "DS3234"** e a outra _(ArduinoTempo)_ para RTC's do tipo: **"MD_MAX72xx"**.

  - Depois de fazeres "fork" a uma delas, dá upload para o teu Arduino o ficheiro .ino que se encontra dentro de ambas as pastas e os teus LED's deverão acender!
- ### Mudança de tempo
  - Com a humanidade a lutar constantemente para esta causa o tempo restante por vezes varia e é preciso atualiza-lo. Para mudares o tempo restante basta mudares uma variavel que representa a data final, ou seja, a data que teoricamente o tempo chegaria a 0 anos, 0 dias, 0 horas...

    - **Em ambos os RTC's a mudança de data faz-se de forma semelhante:**

      1.  Abre o código do ficheiro **ArduinoTempo.ino.** ou **Codigo_outro_RTC.ino** de acordo com o teu RTC.
      2.  Dentro do codigo, navega até encontrares uma variavel do tipo "**struct date**" chamada "**limitdate**":
          <img  src="https://prnt.sc/1wlqh52"  alt="CodigoAmudar"  width="300"/>
      3.  Para mudares para o tempo pretendido modifica os dias, meses, horas, dias, minutos, segundos para a nova data final!
      4.  Envia para o teu Arduino!
      5.  Parabéns mudaste a data final!
