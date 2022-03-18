const loginForm = document.getElementById("login-form");
const loginButton = document.getElementById("login-form-submit");
const loginErrorMsg = document.getElementById("login-error-msg");

console.log(loginButton);
var essai = 3;

loginButton.addEventListener("click", (e) => {
	e.preventDefault();
	const username = loginForm.username.value;
	const password = loginForm.password.value;

	if (username === "user" && password === "passwd") {
		alert("You have successfully logged in.");
		location.reload();
	} else {
		if (essai == 1) {
			alert("Vous êtes bloqué");
		} else {
			essai--;
			alert("Il vous reste " + essai + " essai(s)");
		}

	}
	fetch('http://192.168.4.1/api/login', {
		mode: 'no-cors',
		method: 'POST',
		body: JSON.stringify({
			username: username,
			password: password
		}),
		headers: {
			'Content-type': 'application/json; charset=UTF-8'
		}
	}).then(function (response) {
		if (response.ok) {
			return response.json();
		}
		return Promise.reject(response);
	}).then(function (data) {
		console.log(data);
	}).catch(function (error) {
		console.warn('Something went wrong.', error);
	});
})





