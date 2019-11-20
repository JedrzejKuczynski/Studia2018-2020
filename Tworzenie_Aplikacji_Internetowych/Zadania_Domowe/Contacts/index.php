<?php
$mysqli = new mysqli("sql7.freemysqlhosting.net", "sql7312902", "FILRMpNW83", "sql7312902", 3306);
if ($mysqli->connect_errno) {
    echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
    exit();
}

if (!$mysqli->set_charset("utf8")) {
    echo "Error loading character set utf8: " .  $mysqli->error;
    exit();
}

// FORMULARZ
$id = isset($_POST['id']) ? $_POST['id'] : null;
$firstname = isset($_POST['firstname']) ? $_POST['firstname'] : null;
$lastname = isset($_POST['lastname']) ? $_POST['lastname'] : null;
$phone = isset($_POST['phone']) ? $_POST['phone'] : null;
$image = isset($_POST['image']) ? $_POST['image'] : null;

if ($firstname && $lastname && $phone) {

    if ($id) {
        $stmt = $mysqli->prepare("UPDATE contact SET firstname=?, lastname=?, phone=?, image=? WHERE id = ?");
        $stmt->bind_param("ssssi", $firstname, $lastname, $phone, $image, $id);
    } else {
        $stmt = $mysqli->prepare("INSERT INTO contact(firstname, lastname, phone, image) VALUES (?, ?, ?, ?)");
        $stmt->bind_param("ssss", $firstname, $lastname, $phone, $image);
    }

    if (!$stmt->execute()) {
        echo "Execute failed: (" . $stmt->errno . ") " . $stmt->error;
    } else {
        $firstname = '';
        $lastname = '';
        $phone = '';
        $image = '';
        $id = null;
    }
}

// LINKI - UPDATE & DELETE
$operation = isset($_GET['o']) ? $_GET['o'] : null;
$id = isset($_GET['id']) ? $_GET['id'] : null;
$column = isset($_GET['column']) ? $_GET['column'] : "lastname";
$sort_order = isset($_GET['order']) && strtolower($_GET['order']) == 'desc' ? "DESC" : "ASC";
$asc_or_desc = $sort_order == 'ASC' ? 'desc' : 'asc';
$arrow = str_replace(array('ASC','DESC'), array('up','down'), $sort_order); 

if ($operation && $id) {
    $res = $mysqli->query("SELECT * FROM contact WHERE id  = " . $id);
    $found = $res->fetch_assoc();

    switch ($operation) {
        case 'update':
            $firstname = $found['firstname'];
            $lastname = $found['lastname'];
            $phone = $found['phone'];
            $image = $found['image'];
            break;
        case 'delete':
        	$del = "DELETE FROM contact WHERE id=" . $id;
        	if($mysqli->query($del)){
        		echo "Record was deleted successfully!";
        	} else{
        		echo "Error occured while deleting record: " . $mysqli->error;
        	}
            break;
        default:
      		break;
    }
}


$res = $mysqli->query("SELECT * FROM contact ORDER BY " . $column . ' ' . $sort_order);
?>

    <!DOCTYPE html>
    <html lang="en">

    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta http-equiv="X-UA-Compatible" content="ie=edge">
        <title>PhoneBook</title>
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
    </head>

    <body>

        <form action="index.php" method="post">
            <label for="firstname">First name: <input name="firstname" value="<?= $firstname ?>" type="text"></label>
            <label for="lastname">Last name: <input name="lastname" value="<?= $lastname ?>" type="text"></label>
            <label for="phone">Phone: <input name="phone" value="<?= $phone ?>" type="text"></label>
            <label for="image">Image: <input name="image" value="<?= $image ?>" type="text"></label>
            <input type="hidden" name="id" value="<?= $id ?>">
            <input type="submit" value="Save">
        </form>

        <br>

        <table border="1" cellpadding="10" cellspacing="0">
        	<tr>
        		<th><a href="index.php?&column=id&order=<?php echo $asc_or_desc; ?>">ID Number<i class="fa fa-sort<?php echo $column == 'id' ? '-' . $arrow : ''; ?>"></i></a></th>
        		<th><a href="index.php?&column=firstname&order=<?php echo $asc_or_desc; ?>">First Name<i class="fa fa-sort<?php echo $column == 'firstname' ? '-' . $arrow : ''; ?>"></i></a></th>
        		<th><a href="index.php?&column=lastname&order=<?php echo $asc_or_desc; ?>">Last Name<i class="fa fa-sort<?php echo $column == 'lastname' ? '-' . $arrow : ''; ?>"></i></a></th>
        		<th><a href="index.php?&column=phone&order=<?php echo $asc_or_desc; ?>">Phone Number<i class="fa fa-sort<?php echo $column == 'phone' ? '-' . $arrow : ''; ?>"></i></a></th>
        		<th>Image</th>
        		<th>Action</th>
        	</tr>
            <?php while ($row = $res->fetch_assoc()): ?>
                <tr>
                    <td><?= $row['id'] ?></td>
                    <td><?= $row['firstname'] ?></td>
                    <td><?= $row['lastname'] ?></td>
                    <td><?= $row['phone'] ?></td>
                    <td>
                        <img src="<?= $row['image'] ?>" style="width: 100%; height: auto; max-width: 100px;">
                    </td>
                    <td>
                        <a href="index.php?o=update&id=<?= $row['id'] ?>"><i class="fa fa-pencil" aria-hidden="true"></i></a>
                        <a href="index.php?o=delete&id=<?= $row['id'] ?>"><i class="fa fa-trash" aria-hidden="true"></i></a>
                    </td>
                </tr>
            <?php endwhile; ?>
        </table>

    </body>

    </html>
