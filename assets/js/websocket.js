// Récupère le chemin du fichier HTML
var path = window.location.pathname;
// Récupère le nom du fichier HTML
var page = path.split("/").pop();
// Création du websocket
const socket = new WebSocket('ws://192.168.64.91:12345');
// Définition du status
const statusSocket = document.getElementById('status');
// Réponse JSON
var reponseJSON = null;
// Déclaration d'une variable pour stocker l'ID de l'intervalle
let intervalID;

// Interface
var r = document.querySelector('#r');
var g = document.querySelector('#g');
var b = document.querySelector('#b');
var w = document.querySelector('#w');
var adress = document.querySelector('#adress');
var r_out = document.querySelector('#r_out');
var g_out = document.querySelector('#g_out');
var b_out = document.querySelector('#b_out');
var w_out = document.querySelector('#w_out');
var adress_out = document.querySelector('#adress_out');
var monCheckbox = document.getElementById('changementAuto');

r.addEventListener('change', function() {
  r_out.value = r.value;
}, false);

r.addEventListener('input', function() {
  r_out.value = r.value;
}, false);

g.addEventListener('change', function() {
  g_out.value = g.value;
}, false);

g.addEventListener('input', function() {
  g_out.value = g.value;
}, false);

b.addEventListener('change', function() {
  b_out.value = b.value;
}, false);

b.addEventListener('input', function() {
  b_out.value = b.value;
}, false);

w.addEventListener('change', function() {
    w_out.value = w.value;
}, false);
  
w.addEventListener('input', function() {
    w_out.value = w.value;
}, false);

adress.addEventListener('change', function() {
    adress_out.value = adress.value;
}, false);
  
adress.addEventListener('input', function() {
    adress_out.value = adress.value;
}, false);

// Écouter l'événement de clic sur le checkbox
monCheckbox.addEventListener('click', () => {
    // Inverser la valeur du checkbox à chaque clic
    console.log(monCheckbox.checked);
});

// Tentative Connexion
if (socket.readyState == 0)
{
    statusSocket.innerHTML = "Status : Le socket a été créé ! En attente de connexion ...";
}

// Fonction pour envoyer au serveur
function requestUser() 
{
  let msg = {
    type: "trameDMX512",
    adressLum: parseInt(adress.value),
    redValue: parseInt(r.value),
    greenValue : parseInt(g.value),
    blueValue: parseInt(b.value),
    whiteValue: parseInt(w.value), 
    changeAutoLum: monCheckbox.checked,
  };

  socket.send(JSON.stringify(msg));
}

// Selon ou on se situe
if (page == "accueil.php")
{
  // Connexion Ouverte
  socket.addEventListener('open', () => {
    statusSocket.innerHTML = "Status : Connecter !";

    // Appeler requestUser toutes les 1000 ms (1 seconde)
    intervalID = setInterval(requestUser, 1000);
  });

  /* SI  ON SOUHAITE RECEVOIR DES DONNEES
  socket.onmessage = function (evt) // Lorsque le serveur répond
  {
    //reponseJSON = JSON.parse(evt.data); // On recupere en format json
    console.log(evt.data);
  }
  */
 
  // Gérer la déconnexion du WebSocket
  socket.addEventListener('close', () => {
    statusSocket.innerHTML = "Status : Déconnecté !";
        
    // Arrêter l'envoi de trames en effaçant l'intervalle
    clearInterval(intervalID);
  });
}