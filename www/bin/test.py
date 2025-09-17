#!/usr/bin/env python3

import os
import sys
import re
import base64

def parse_multipart_data(data, boundary):
    """Simple multipart parser for file uploads"""
    parts = data.split(f'--{boundary}'.encode())
    files = []
    fields = {}

    for part in parts:
        if not part or part == b'--\r\n' or part == b'--':
            continue

        # Find the headers section
        if b'\r\n\r\n' in part:
            headers_section, content = part.split(b'\r\n\r\n', 1)
            content = content.rstrip(b'\r\n')

            headers = headers_section.decode('utf-8', errors='ignore')

            # Parse Content-Disposition header
            if 'Content-Disposition' in headers:
                # Extract field name
                name_match = re.search(r'name="([^"]*)"', headers)
                filename_match = re.search(r'filename="([^"]*)"', headers)

                if filename_match:
                    # It's a file upload
                    field_name = name_match.group(1) if name_match else 'unknown'
                    filename = filename_match.group(1)

                    # Extract content type
                    content_type_match = re.search(r'Content-Type:\s*([^\r\n]*)', headers)
                    content_type = content_type_match.group(1) if content_type_match else 'application/octet-stream'

                    files.append({
                        'field_name': field_name,
                        'filename': filename,
                        'content_type': content_type,
                        'content': content,
                        'size': len(content)
                    })
                elif name_match:
                    # It's a regular form field
                    field_name = name_match.group(1)
                    fields[field_name] = content.decode('utf-8', errors='ignore')

    return fields, files

def main():
    # Print HTTP headers
    print()  # Required empty line

    method = os.environ.get('REQUEST_METHOD', 'UNKNOWN')
    content_type = os.environ.get('CONTENT_TYPE', '')
    content_length_str = os.environ.get('CONTENT_LENGTH', '0').strip()

    # Handle empty or invalid CONTENT_LENGTH
    try:
        content_length = int(content_length_str) if content_length_str else 0
    except ValueError:
        content_length = 0

    print('''<!DOCTYPE html>
<html>
<head>
    <title>CGI File Upload Test</title>
    <style>
        body { font-family: Arial, sans-serif; max-width: 800px; margin: 0 auto; padding: 20px; }
        .info { background: #e7f3ff; padding: 15px; margin: 10px 0; border-radius: 5px; }
        .success { background: #d4edda; padding: 15px; margin: 10px 0; border-radius: 5px; }
        .error { background: #f8d7da; padding: 15px; margin: 10px 0; border-radius: 5px; }
        .file-info { background: #f8f9fa; padding: 15px; margin: 10px 0; border-radius: 5px; border: 1px solid #dee2e6; }
        .image-preview {
            background: white;
            padding: 20px;
            margin: 15px 0;
            border-radius: 8px;
            border: 2px solid #28a745;
            text-align: center;
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
        }
        .preview-image {
            max-width: 500px;
            max-height: 400px;
            border: 2px solid #dee2e6;
            border-radius: 5px;
            box-shadow: 0 2px 8px rgba(0,0,0,0.15);
        }
        pre { background: #f5f5f5; padding: 10px; overflow-x: auto; }
        .upload-form { border: 2px dashed #ccc; padding: 20px; margin: 20px 0; }
    </style>
</head>
<body>
    <h1>CGI File Upload Test</h1>''')

    print(f'''
    <div class="info">
        <strong>Request Method:</strong> {method}<br>
        <strong>Content Type:</strong> {content_type}<br>
        <strong>Content Length:</strong> {content_length} bytes
    </div>''')

    if method == 'POST' and content_length > 0:
        try:
            # Read all POST data
            post_data = sys.stdin.buffer.read(content_length)

            if 'multipart/form-data' in content_type:
                # Extract boundary
                boundary_match = re.search(r'boundary=([^;]+)', content_type)
                if boundary_match:
                    boundary = boundary_match.group(1).strip()

                    # Parse multipart data
                    fields, files = parse_multipart_data(post_data, boundary)

                    print('<div class="success"><h2>Upload Successful!</h2></div>')

                    # Display form fields
                    if fields:
                        print('<h3>Form Fields:</h3>')
                        for field_name, value in fields.items():
                            print(f'<div class="file-info"><strong>{field_name}:</strong> {value}</div>')

                    # Display uploaded files
                    if files:
                        print('<h3>Uploaded Files:</h3>')
                        for i, file_info in enumerate(files):
                            print(f'''
                            <div class="file-info">
                                <strong>Field Name:</strong> {file_info['field_name']}<br>
                                <strong>Filename:</strong> {file_info['filename']}<br>
                                <strong>Content Type:</strong> {file_info['content_type']}<br>
                                <strong>Size:</strong> {file_info['size']} bytes<br>
                            ''')

                            # If it's an image, try to display it
                            if file_info['content_type'].startswith('image/') and file_info['size'] > 0:
                                try:
                                    # Convert to base64 for display
                                    b64_data = base64.b64encode(file_info['content']).decode('utf-8')
                                    print(f'''
                                    </div>
                                    <div class="image-preview">
                                        <h4>Image Preview:</h4>
                                        <img src="data:{file_info['content_type']};base64,{b64_data}"
                                             class="preview-image"
                                             alt="Uploaded image preview">
                                        <p><em>Successfully uploaded: {file_info['filename']}</em></p>
                                    </div>
                                    <div class="file-info">
                                    ''')
                                except Exception as e:
                                    print(f'<strong>Preview Error:</strong> {e}<br>')

                            # Show first 100 bytes as hex for binary files
                            elif file_info['size'] > 0:
                                hex_preview = file_info['content'][:100].hex()
                                print(f'<strong>Binary Data (first 100 bytes):</strong><br><pre>{hex_preview}</pre>')

                            print('</div>')

                    if not fields and not files:
                        print('<div class="error">No files or fields found in upload</div>')

                else:
                    print('<div class="error">Could not find boundary in multipart data</div>')

            else:
                # Handle regular POST data
                try:
                    decoded_data = post_data.decode('utf-8')
                    print(f'<div class="success"><h3>POST Data Received:</h3><pre>{decoded_data}</pre></div>')
                except:
                    print(f'<div class="info"><h3>Binary POST Data:</h3><pre>{post_data[:500].hex()}...</pre></div>')

        except Exception as e:
            print(f'<div class="error"><strong>Error processing upload:</strong> {e}</div>')

    # Upload form
    print('''
    <div class="upload-form">
        <h2>Test File Upload</h2>
        <form method="POST" enctype="multipart/form-data">
            <p>
                <label><strong>Description:</strong><br>
                <input type="text" name="description" placeholder="Describe your file">
                </label>
            </p>
            <p>
                <label><strong>Select Image/File:</strong><br>
                <input type="file" name="upload_file" accept="image/*,*/*">
                </label>
            </p>
            <p>
                <label><strong>Category:</strong><br>
                <select name="category">
                    <option value="photo">Photo</option>
                    <option value="document">Document</option>
                    <option value="other">Other</option>
                </select>
                </label>
            </p>
            <p>
                <input type="submit" value="Upload File"
                       style="background: #007bff; color: white; padding: 10px 20px; border: none; border-radius: 4px; cursor: pointer;">
            </p>
        </form>
    </div>

    <div class="info">
        <h3>Testing Tips:</h3>
        <p><strong>With curl (image upload):</strong></p>
        <pre>curl -X POST -F "upload_file=@/path/to/image.jpg" -F "description=Test image" http://localhost/upload_test.py</pre>

        <p><strong>With curl (any file):</strong></p>
        <pre>curl -X POST -F "upload_file=@/path/to/file.pdf" http://localhost/upload_test.py</pre>

        <p><strong>Supported:</strong> Images (with preview), PDFs, text files, binary files</p>
    </div>

</body>
</html>''')

if __name__ == "__main__":
    main()
