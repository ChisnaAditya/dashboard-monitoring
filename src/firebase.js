import { initializeApp } from "firebase/app";
import { getAuth } from "firebase/auth";
import { getDatabase } from "firebase/database";

// const firebaseConfig = {
//   apiKey: "AIzaSyAxmczt_l4HYCNTrm7aIOXswdyW3gGyW1Q",
//   authDomain: "naufalarif-20e6a.firebaseapp.com",
//   projectId: "naufalarif-20e6a",
//   storageBucket: "naufalarif-20e6a.appspot.com",
//   messagingSenderId: "886579045240",
//   appId: "1:886579045240:web:f364d4a7e94662b395de8d",
// };

const firebaseConfig = {
  apiKey: "AIzaSyAxmczt_l4HYCNTrm7aIOXswdyW3gGyW1Q",
  authDomain: "naufalarif-20e6a.firebaseapp.com",
  databaseURL:
    "https://naufalarif-20e6a-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "naufalarif-20e6a",
  storageBucket: "naufalarif-20e6a.appspot.com",
  messagingSenderId: "886579045240",
  appId: "1:886579045240:web:f364d4a7e94662b395de8d",
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
export const auth = getAuth();
export const database = getDatabase();
