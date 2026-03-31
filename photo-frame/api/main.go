package main

import (
	"bytes"
	"fmt"
	"io"
	"log"
	"mime/multipart"
	"net/http"
	"os"
	"os/exec"
)

func uploadToFrame(filename string) error {
	log.Printf("Starting file upload...")

	// 1. Open the file
	file, err := os.Open("tmp.bmp")
	if err != nil {
		return fmt.Errorf("failed to open file: %w", err)
	}
	defer file.Close()

	// 2. Prepare the buffer and multipart writer
	var requestBody bytes.Buffer
	writer := multipart.NewWriter(&requestBody)

	// Add extra form fields (optional)
	if err := writer.WriteField("description", "A photo of my cat"); err != nil {
		return fmt.Errorf("failed to write form field: %w", err)
	}

	// 3. Add the file field
	part, err := writer.CreateFormFile("data", "tmp.bmp")
	if err != nil {
		return fmt.Errorf("failed to create form file: %w", err)
	}

	if _, err := io.Copy(part, file); err != nil {
		return fmt.Errorf("failed to copy file to buffer: %w", err)
	}

	// Close the writer to finalize the boundary before sending
	writer.Close()

	// 4. Create the request
	req, err := http.NewRequest("POST", "http://192.168.176.177/upload", &requestBody)
	if err != nil {
		return fmt.Errorf("failed to create request: %w", err)
	}

	// Set the header with the generated boundary
	req.Header.Set("Content-Type", writer.FormDataContentType())

	// 5. Send the request and AWAIT the response
	client := &http.Client{}
	resp, err := client.Do(req) // <-- Code stops here until server answers
	if err != nil {
		return fmt.Errorf("failed to send request: %w", err)
	}
	defer resp.Body.Close()

	// 6. Read and log the actual server response
	responseBytes, err := io.ReadAll(resp.Body)
	if err != nil {
		return fmt.Errorf("failed to read response body: %w", err)
	}

	log.Printf("Server responded with status: %s", resp.Status)
	log.Printf("Server response body: %s", string(responseBytes))
	log.Printf("File upload done!")

	log.Printf("Cleaning up...")
	os.Remove(filename)
	os.Remove("tmp.bmp")

	return nil
}

func uploadFile(w http.ResponseWriter, r *http.Request) {
	log.Printf("Recieving file...")

	// Limit upload size (e.g., 10 MB)
	r.ParseMultipartForm(10 << 20)

	// Retrieve file from form data
	file, handler, err := r.FormFile("data")
	if err != nil {
		http.Error(w, "Error retrieving the file", http.StatusBadRequest)
		return
	}
	defer file.Close()

	// Create a local destination file
	dst, err := os.Create(handler.Filename)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}
	defer dst.Close()

	// Copy the uploaded file to the destination
	if _, err := io.Copy(dst, file); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	log.Printf("Converting to bmp...")
	cmd := exec.Command("magick", handler.Filename, "BMP3:tmp.bmp")
	_, err = cmd.Output()
	if err != nil {
		log.Fatal(err)
	}

	// upload in background and return immediately
	log.Printf("Uploading to frame...")
	go uploadToFrame(handler.Filename)
	fmt.Fprintf(w, "OK")
}

func main() {
	http.HandleFunc("/upload", uploadFile)
	log.Fatal(http.ListenAndServe(":8080", nil))
}
