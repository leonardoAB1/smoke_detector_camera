<script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
(function ($) {
  "use strict";

  // MENU
  $('#sidebarMenu .nav-link').on('click', function () {
    $("#sidebarMenu").collapse('hide');
  });

  // CUSTOM LINK
  $('.smoothscroll').click(function () {
    var el = $(this).attr('href');
    var elWrapped = $(el);
    var header_height = $('.navbar').height();

    // Llamada a la función scrollToDiv
    scrollToDiv(elWrapped, header_height);
    return false;

    function scrollToDiv(element, navHeight) {
      var offset = element.offset();
      var offsetTop = offset.top;
      var totalScroll = offsetTop - navHeight;

      // Desplazamiento suave usando animate()
      $('body,html').animate({
        scrollTop: totalScroll
      }, 300);
    }
  });
});
const darkModeToggle = document.getElementById('dark-mode-toggle');

  // Agrega un evento de escucha de clic
  darkModeToggle.addEventListener('click', function() {
    // Obtén el elemento raíz
    const rootElement = document.documentElement;
  
    // Alterna la clase 'dark-mode' en el elemento raíz
    rootElement.classList.toggle('dark-mode');
  });
      function mostrarFechaActual() {
      var fecha = new Date(); // Obtiene la fecha actual
      var dia = fecha.getDate(); // Obtiene el día del mes
      var mes = fecha.getMonth() + 1; // Obtiene el mes (los meses comienzan en 0, por eso se le suma 1)
      var anio = fecha.getFullYear(); // Obtiene el año

      // Formatea la fecha en el formato deseado (por ejemplo, dd/mm/yyyy)
      var fechaFormateada = dia + '/' + mes + '/' + anio;

      // Inserta la fecha en el elemento con el id "fecha"
      document.getElementById('fecha').innerHTML = fechaFormateada;
    }

    // Llama a la función mostrarFechaActual al cargar la página
    window.onload = mostrarFechaActual;