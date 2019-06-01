const functions = require('firebase-functions');
const admin = require('firebase-admin');
admin.initializeApp(functions.config().firebase);

// Create and Deploy Your First Cloud Functions
// https://firebase.google.com/docs/functions/write-firebase-functions

var firestore = admin.firestore();

exports.helloWorld = functions.https.onRequest((request, response) => {
    response.status(200).send("Hello from Firebase!");

});


exports.q1opt1 = functions.https.onRequest((request, response) => {
    return firestore.collection("q2").add(
        {
            timestamp: new Date().toISOString(),
            responseIndex: 0
        }
    )
    .then( () => {
        console.log('req complete');
        return response.status(200).send("Vote Added");
    });
});

exports.q1opt2 = functions.https.onRequest((request, response) => {
    return firestore.collection("q2").add(
        {
            timestamp: new Date().toISOString(),
            responseIndex: 1
        }
    )
    .then( () => {
        console.log('req complete');
        return response.status(200).send("Vote Added");
    });
});

exports.q1opt3 = functions.https.onRequest((request, response) => {
    return firestore.collection("q2").add(
        {
            timestamp: new Date().toISOString(),
            responseIndex: 2
        }
    )
    .then( () => {
        console.log('req complete');
        return response.status(200).send( "Vote Added");
    });
});
