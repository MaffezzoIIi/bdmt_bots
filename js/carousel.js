var i = 1;

const changeComponentRight = () => {
  document.getElementById(`${i}`).style.display = "none";
  i++;

  if ( i === 8 ) {
    i = 1;
  }

  document.getElementById(`${i}`).style.display = "block";
};

const changeComponentLeft = () => {
  document.getElementById(`${i}`).style.display = "none";
  i--;

  if ( i === 0 ) {
    i = 7;
  }

  document.getElementById(`${i}`).style.display = "block";
};

document.getElementById('buttonLeft').addEventListener('click', changeComponentLeft);
document.getElementById('buttonRight').addEventListener('click', changeComponentRight);