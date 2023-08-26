let tableIsBuilt = false;
let fetchCalibrationFirstTime = true;
function buildSensorTable(calibrationValues, currentValues) {
  if (!tableIsBuilt) {
    const sensorTableBody = document.getElementById('sensorTableBody');
    for (let channel = 0; channel < 16; channel++) {
      const newRow = document.createElement('tr');

      const sensorIdCell = document.createElement('td');
      sensorIdCell.textContent = `Channel ${channel}`;

      const calibrationCell = document.createElement('td');
      const calibrationInput = document.createElement('input');
      calibrationInput.type = 'number';
      calibrationInput.step = '0.01';
      calibrationInput.value = calibrationValues[channel];
      calibrationInput.className = 'calibration-input';
      calibrationCell.appendChild(calibrationInput);

      const sensorValueCell = document.createElement('td');
      sensorValueCell.className = 'sensor-value';
      sensorValueCell.textContent = `${currentValues[channel]} A`;

      const calibrateCell = document.createElement('td');
      const calibrateButton = document.createElement('button');
      calibrateButton.textContent = 'Calibrate';
      calibrateButton.onclick = () => calibrate(channel, calibrationInput);
      calibrateCell.appendChild(calibrateButton);

      newRow.appendChild(sensorIdCell);
      newRow.appendChild(calibrationCell);
      newRow.appendChild(sensorValueCell);
      newRow.appendChild(calibrateCell);
      
      sensorTableBody.appendChild(newRow);
    }

    tableIsBuilt = true;
  }
  else
  {
    updateTable(currentValues);
  }
}

document.addEventListener('DOMContentLoaded', () => {
    // Function to fetch and update calibration and current values
    function updateValues() {
      // Fetch calibration and current values
      fetch('/calibration')
        .then(response => response.json())
        .then(calibrationValues => {
          fetch('/current')
            .then(response => response.json())
            .then(currentValues => {
              buildSensorTable(calibrationValues, currentValues);
            });
        });
    }
    // Initial update and periodic update (every 5 seconds)
    updateValues();
    setInterval(updateValues, 5000);
  });
  
  function updateTable( currentValues) {
    if (!tableIsBuilt) return;
    const sensorTableBody = document.getElementById('sensorTableBody');
    const sensorValueLabels = sensorTableBody.getElementsByClassName('sensor-value');
    for (let channel = 0; channel < 16; channel++) {
      // Update the sensor value label for the channel
      sensorValueLabels[channel].textContent = currentValues[channel] + ' A';
    }
  }
  
  function calibrate(channel, calibrationInput) {
    const calibrationValue = parseFloat(calibrationInput.value);
     // Create JSON payload
    const payload = {
      channel,
      calibrationValue
    };
    // Send a POST request to calibrate the channel
    fetch('/calibrate', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    })
      .then(response => response.text())
      .then(message => {
        alert(message);
      });
  }
  