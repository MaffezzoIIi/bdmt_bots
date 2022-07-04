const titulos_list = ['titulo_semaforo', 'titulo_de_emergencia', 'titulo_frente_tras',  'titulo_controlado'];
const img_list = ['semaforo', 'luz_emergencia', 'motor_frente_tras', 'simulador_segue_trilha'];

var t = 0;
const changeTitlesRight = () => {

  document.getElementById(`${titulos_list[t]}`).style.display = "none";
  document.getElementById(`${img_list[t]}`).style.display = "none";
  
  t++;

  if (t === 4 ) {
    t = 0;
  }

  document.getElementById(`${titulos_list[t]}`).style.display = "block";
  document.getElementById(`${img_list[t]}`).style.display = "flex";
}

const changeTitlesLeft = () => {

  document.getElementById(`${titulos_list[t]}`).style.display = "none";
  document.getElementById(`${img_list[t]}`).style.display = "none";
  
  if (t === 0 ) {
    t = 3;
  } else {
    t--;
  }

  document.getElementById(`${titulos_list[t]}`).style.display = "block";
  document.getElementById(`${img_list[t]}`).style.display = "flex";
}

document.getElementById('buttonLeftCircuits').addEventListener('click', changeTitlesLeft);
document.getElementById('buttonRightCircuits').addEventListener('click', changeTitlesRight);