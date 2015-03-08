/**
 * Welcome to Pebble.js!
 *
 * This is where you write your app.
 */
var ajax = require('ajax');
var UI = require('ui');

var card = new UI.Card({
  title:'Weather',
  subtitle:'Meming...'
});

// Display the Card
card.show();


// Construct URL
var cityName = 'Chicago';
var URL = 'http://api.openweathermap.org/data/2.5/weather?q=' + cityName;

ajax(
  {
    url: URL,
    type: 'json'
  },
  function(data) {
    // Success!
    console.log('Successfully fetched weather data!');
  },
  function(error) {
    // Failure!
    console.log('Failed fetching weather data: ' + error);
  },
  function(data) {
  // Success!
  console.log('Successfully fetched weather data!');

  // Extract data
  var location = data.name;
  var temperature = Math.round(data.main.temp - 273.15) + 'C';

  // Always upper-case first letter of description
  var description = data.weather[0].description;
  description = description.charAt(0).toUpperCase() + description.substring(1);
  
  card.subtitle(location + ', ' + temperature);
card.body(description);
}

  
);



// Show to user
